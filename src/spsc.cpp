#include <cstddef>
#include <iostream>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <vector>
#include <thread>

// 实现生产者消费者

using namespace std;

const int MAXNUM = 50;
const int CAPACITY = 10;
condition_variable cv_full;
condition_variable cv_empty;
mutex mtx;
deque<int> buffer;

void producer(){
    for(int i=0; i < MAXNUM; i++)    {
        std::unique_lock<std::mutex> lock(mtx);

        cv_full.wait(lock, [] {return buffer.size() <= CAPACITY;});

        cout << "生产了: " << i << endl;
        buffer.push_back(i);

        cv_empty.notify_one();
    }
}

void consumer(){
    while(true){
        std::unique_lock<std::mutex> lock(mtx);

        cv_empty.wait(lock, []{return buffer.size() > 0;});

        int val = buffer.front();
        buffer.pop_front();
        cout << "消费了: " << val << endl;

        cv_full.notify_one();
        
        if(val == MAXNUM - 1){
            break;
        }
    }
}

int main(){
    cin.tie(nullptr);
    cout.tie(nullptr);

    cout << "生产者和消费者开始运行" << endl;

    vector<thread> threads;
    threads.emplace_back(consumer);
    threads.emplace_back(producer);

    for(auto& t : threads){
        t.join();
    }

    cout << "生产者和消费者结束运行" << endl;
}