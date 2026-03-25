#include <iostream>
#include <cassert>
#include <atomic>
#include <mutex>
#include <optional>
#include <thread>

using namespace std;

class SPSC{
public:
    const static int CAPACITY = 50;

    bool push(int value){
        int h = head.load(std::memory_order_acquire);
        int t = tail.load(std::memory_order_acquire);

        int next_h = (h + 1) % CAPACITY;
        if(next_h == t) {return false;}

        data[h] = value;

        head.store(next_h, std::memory_order_release);

        return true;
    }

    optional<int>pop(){
        int h = head.load(std::memory_order_acquire);
        int t = tail.load(std::memory_order_acquire);

        if(t == h) {return nullopt;}

        int res = data[t];

        tail.store((t + 1) % CAPACITY, std::memory_order_release);

        return res;
    }

private:
    int data[CAPACITY];
    // head == t 时，判定为缓冲区空, (head + 1 % CAPACITY) == t 时，判定为满
    atomic<int> head{0};
    atomic<int> tail{0};
} spsc;

mutex iomtx;

void producer(){
    // 写入 -1 就意味着结束
    int n = 500 * SPSC::CAPACITY;
    assert(n > 0);

    int write_fault = 0;
    for(int i=0; i < n; i++){
        while(! spsc.push(i)){
            ++write_fault;
        }
    }

    while(! spsc.push(-1));

    {
        unique_lock<mutex> lock(iomtx);
        cout << "producer write fault: " << write_fault << " total write: " << n << endl;
    }
}

void consumer(){
    int read_fault = 0;
    while(true){
        auto opt_i = spsc.pop();

        if(opt_i.has_value() && opt_i.value() != -1){

        }else if(opt_i.has_value()){
            break;
        }else{
            ++read_fault;
        }
    }

    {
        unique_lock<mutex> lock(iomtx);
        cout << "consumer read fault: " << read_fault << endl;
    }
}

int main(){
    cout << "program begin" << endl;

    thread con_thread = thread(consumer);
    thread prod_thread = thread(producer);

    con_thread.join();
    prod_thread.join();

    cout << "program end" << endl;

    return 0;
}