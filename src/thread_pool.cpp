#include <chrono>
#include <cstddef>
#include <exception>
#include <functional>
#include <future>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <deque>
#include <iostream>
#include <memory>

using std::mutex;
using std::thread;
using std::condition_variable;
using std::vector;
using std::unique_lock;
using std::deque;


// 比较难，主要是在是lambda 拷贝那里， std::function<> 要求传入的内容可拷贝，如果传入
// 一个 lambda，那么要求 lambda 也可拷贝

template<std::size_t Capacity>
class ThreadPool{
    static_assert( Capacity > 0 && Capacity < 100); // 测试程序，保险起见不允许过大的线程数量

public:
    ThreadPool(): stop_(false)
    {
        workers_.reserve(Capacity);
        for(int i = 0; i < Capacity; ++i){
            workers_.emplace_back(
                [this]() { // 为啥非要这样写？
                    worker_loop();
                }
            );
        }
    }

    ThreadPool(ThreadPool& other) = delete;
    ThreadPool& operator=(ThreadPool& rhs) = delete;

    ~ThreadPool(){
        shutdown();
    }

    void shutdown(){
        {
            unique_lock<mutex> lock(mtx_);
            if(stop_){
                return;
            }

            stop_ = true;
        }

        cv_.notify_all();

        for(auto& worker : workers_){
            if(worker.joinable()){
                worker.join();
            }
        }
    }

    template<typename Func, typename... Args>
    auto submit(Func&& func, Args&&... args) -> std::future<std::invoke_result_t<Func, Args...>>{
        using ReturnType = std::invoke_result_t<Func, Args...>;

        // std::function requires a copyable callable.  A promise is move-only,
        // so keep it behind shared_ptr before putting the task in the queue.
        auto promise = std::make_shared<std::promise<ReturnType>>();
        auto task = std::bind(
            func,
            args...
        );
        auto future = promise->get_future();

        unique_lock<mutex> lock(mtx_);

        tasks_.emplace_back(
            [promise, task]() mutable {
            try{
                if constexpr (std::is_void_v<ReturnType>){
                    task();
                    promise->set_value();
                }else{
                    promise->set_value( task() );
                }
            }catch(...){
                promise->set_exception(std::current_exception());
            }
        });

        cv_.notify_one();

        return future;
    }

private:

    void worker_loop(){
        while(true){
            std::function<void()> task;
            {
                unique_lock<mutex> lock(mtx_);
                cv_.wait(lock, [this]{
                    return stop_ || (!tasks_.empty());
                });

                if(stop_){
                    break;
                }
 
                // 从积攒的任务中选择一个
                task = std::move( tasks_.front() );
                tasks_.pop_front();
            }

            // task 必定不抛异常
            task();
        } 
    }

    mutex mtx_;
    condition_variable cv_;
    vector<thread> workers_;
    deque<std::function<void()>> tasks_;
    bool stop_;
};

int main(){
    ThreadPool<4> tp;

    vector<std::future<int>> futures;

    for(int i = 0; i < 8; ++i){
        futures.emplace_back(
            tp.submit([](int i){
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                return i * i;
            }, i)
        );
    }

    for(auto& f : futures){
        int result = f.get();
        std::cout << result << std::endl;
    }

    auto exception_future = tp.submit([](){
        throw std::runtime_error("run time error");
    });

    try{
        exception_future.get();
    } catch (std::exception& e){
        std::cout << "got execption: " << e.what() << std::endl;
    }

    return 0;
}
