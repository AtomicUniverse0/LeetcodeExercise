
#include <atomic>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <sstream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>

using std::string;
using std::mutex;
using std::thread;
using std::vector;

// 一个 mpmc 的无锁队列
/**
重点学习:
1. 无锁队列的成员变量设计，尤其是版本号的使用，以及对齐机制，inplacement new
2. 实现完美转发
3. 多线程程序的编写要诀
*/

template<class T, std::size_t Capacity>
class MPMCQueue{
    static_assert(Capacity > 0 && ( (Capacity & (Capacity - 1) )== 0) );

public:
    MPMCQueue(){
        // 初始化每个 Cell 的 version_;
        for(uint32_t i = 0; i < Capacity; ++i){
            queue_[i].version_.store(i, std::memory_order_release);
        }
    }
    
    // TODO: 这个接口可优化成完美转发
    bool try_push(T&& elem);

    // 注意这个接口，结果存放在参数里了
    bool try_pop(T& result);

private:

    // 一个Cell保管了一个可以容纳 对象T 的内存空间，并且通过版本号机制来处理并发问题
    class Cell{
    public:
        alignas(T) std::byte memory_addr_[sizeof(T)];
        std::atomic<std::size_t> version_; // 版本号，防止ABA问题

        T* addr_ptr(){
            return reinterpret_cast<T*>(&memory_addr_);
        }
    };

    static const std::size_t MASK = Capacity - 1;
    Cell queue_[Capacity];
    std::atomic<std::size_t> head_;
    std::atomic<std::size_t> tail_;
};

/**
在 MPMC 场景下，要想实现无锁的安全队列，需要考虑几个事之间的顺序:

先查看并更新pos，再写入 elem，再更新 version， 更新完 version后相当于可读了，所以要放到最后
*/
template<class T, std::size_t Capacity>
bool MPMCQueue<T, Capacity>::try_push(T&& elem){
    std::size_t index = tail_.load(std::memory_order_acquire);
    Cell& cell = queue_[ index & MASK ];
    std::size_t version = cell.version_.load(std::memory_order_acquire);

    int diff = static_cast<int>(version) - static_cast<int>(index);
    if(diff == 0){ // 可写
        if( !tail_.compare_exchange_strong(index, index + 1, std::memory_order_acq_rel)){
            return false;
        }
    } else if( diff > 0){ 
        // version > index ?
        return false; 

        // 
    } else{ // diff < 0
        // version < index，说明上次这个位置写入的元素还未被读取    
        return false; // 队列满了
    }

    // 调用拷贝构造函数 / 移动构造函数
    new (cell.addr_ptr()) T( std::forward<T>(elem) );
    // version 加一 意味着可读了
    assert( cell.version_.compare_exchange_strong(version, version + 1, std::memory_order_acq_rel) );

    return true;
}

/**
如何读取？

先修改 head， 然后读取，然后修改 version

*/
template<class T, std::size_t Capacity>
bool MPMCQueue<T, Capacity>::try_pop(T& elem){
    std::size_t index = head_.load(std::memory_order_acquire);
    Cell& cell = queue_[ index & MASK ];
    std::size_t version = cell.version_.load(std::memory_order_acquire);

    int diff = static_cast<int>(version) - static_cast<int>(index);
    if(diff == 1){ // 可读
        if(!head_.compare_exchange_strong(index, index + 1, std::memory_order_acq_rel)){
            return false;
        }
    }else if(diff > 1){
        // std::cout.flush();
        return false;
    }else{ // diff < 1
        return false; // 这个位置还没被写入，说明队列空了
    }

    elem = *cell.addr_ptr();
    cell.addr_ptr()->~T();
    
    assert( cell.version_.compare_exchange_strong(version, index + Capacity, std::memory_order_acq_rel) ) ;

    return true;
}


int main(){
    MPMCQueue<string, 8> q;
    
    mutex iomtx;

    uint32_t thread_count = 5;
    uint32_t target = 10; // 每个生产者和消费者各自生产或消费 target 个
    
    // 每个生产者读取5个
    auto producer_func = [&q, &iomtx](int id, int target){
        for( int i=0; i < target; ++i){
            std::stringstream ss;
            ss << "producer " << id << " message id " << i << "\n";
            string message = ss.str();
            
            while( !q.try_push(std::move(message))){
                // retry
            }

            // iomtx.lock();
            // std::cout << "produer " << id << " produce " << i <<  std::endl; 
            // iomtx.unlock();
        }

        iomtx.lock();
        std::cout << "produer " << id << " finished" << std::endl; 
        iomtx.unlock();
    };

    auto consumer_func = [&q, &iomtx] (int id, int target) {
        for( int i=0; i < target; ++i){
            string output;
           
            while( !q.try_pop(output)){
                // retry
            }
            
            // iomtx.lock();
            // std::cout << "consumer " << id << " consume message: " << output << std::endl; 
            // iomtx.unlock();
        }

        iomtx.lock();
        std::cout << "consumer " << id << " finish." << std::endl;
        iomtx.unlock();
    };

    vector<thread> consumers;
    for(int i=0; i < thread_count; ++i){
        consumers.emplace_back(consumer_func, i, target);
    }
    

    vector<thread> producers;
    for(int i=0; i < thread_count; ++i){
        producers.emplace_back(producer_func, i , target);
    }

    for(auto& t : producers){
        t.join();
    }

    for(auto& t : consumers){
        t.join();
    }

    return 0;
}