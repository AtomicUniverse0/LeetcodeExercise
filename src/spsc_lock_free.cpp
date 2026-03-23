#include <iostream>
#include <cassert>
#include <atomic>
#include <optional>
#include <thread>

using namespace std;

class SPSC{
public:
    const static int CAPACITY = 3;

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

void producer(){
    // 写入 -1 就意味着结束
    int n = 5 * SPSC::CAPACITY;
    assert(n > 0);

    for(int i=0; i < n; i++){
        while(! spsc.push(i)); 
        // cout << "producer produce " << i << endl;
    }

    while(! spsc.push(-1));
}

void consumer(){
    while(true){
        auto opt_i = spsc.pop();

        if(opt_i.has_value() && opt_i.value() != -1){
            // cout << "consumer consume " << opt_i.value() << endl;
        }else if(opt_i.has_value()){
            break;
        }
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