#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <cassert>

using namespace std;

// 循环打印数字第二版，不够优雅

class PrintConfig{
public:
    mutex mtx_;
    condition_variable cv_;
    
    int current_thread_ = -1;
    const int thread_cnt_ = 3;
    int counter_ = 0;
    const int max_num_ = 100;
};

PrintConfig config;

bool check(){
    return config.counter_ < config.max_num_;
}

void printFunc(const int thread_id, const string& thread_name){
    while(check()){
        unique_lock<mutex> lock(config.mtx_);
        
        config.cv_.wait(lock, [thread_id] {
            return config.current_thread_ == thread_id || !check();
        });

        if(!check()){
            config.cv_.notify_all();
            break;
        }

        cout << thread_name << "输出 " << config.counter_++ << endl;
        config.current_thread_ = (config.current_thread_ + 1) % config.thread_cnt_;
        config.cv_.notify_all();
    }
}


int main(){
    cin.tie(nullptr);
    cout.tie(nullptr);

    vector<string> thread_names{"A", "B", "C", "D"};
    assert(thread_names.size() >= config.thread_cnt_);

    cout << "打印程序开始" << endl;
    // flag_one = true; 
    vector<thread> threads;
    for(int thread_id = 0; thread_id < config.thread_cnt_; ++thread_id){
        threads.emplace_back(printFunc, thread_id, thread_names[thread_id]);
    }

    config.current_thread_ = 0;
    config.cv_.notify_all();

    for(auto& t : threads){
        if(t.joinable()){
            t.join();
        }
    }

    cout << "程序结束" << endl;
    return 0;
}