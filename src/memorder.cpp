#include <atomic>
#include <thread>
#include <vector>
#include <cassert>

using namespace std;

// 简单学习一下内存序，如果使用 relaxed，并且makefile里的clang++有 -fsanitize=thread 参数的话，那么运行时编译起会报错

atomic<bool> ready(false);
int num = 0;

void relax_producer(){
    num = 42;
    // std::atomic_thread_fence(std::memory_order_release);
    ready.store(true, std::memory_order_relaxed);
    // ready.store(true, std::memory_order_release);
}

void relax_consumer(){
    while(!ready.load(std::memory_order_relaxed)){}
    // std::atomic_thread_fence(std::memory_order_acquire);
    // while(!ready.load(std::memory_order_acquire)){}
    assert(num == 42);
}

int main(){

    long i = 10L;
    while(i--){
        num = 0;
        ready.store(false);

        vector<thread> progs;
        progs.emplace_back(relax_consumer);
        progs.emplace_back(relax_producer);

        for(auto& t : progs){
            t.join();
        }
    }

    return 0;
}