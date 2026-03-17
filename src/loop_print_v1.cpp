#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

// 循环打印数字第一版，不够优雅

mutex mtx;
condition_variable cv_one;
condition_variable cv_two;
condition_variable cv_three;

const int MAX_NUM = 100;
int counter = 0;
bool flag_one = false;
bool flag_two = false;
bool flag_three = false;

bool static inline shouldContinue(){
    return counter < MAX_NUM;
}

void noOne(){
    string name = "A线程";

    while( shouldContinue() ){
        unique_lock<mutex> lock(mtx);

        cv_one.wait(lock, []{return flag_one;});
        flag_one = false;
        flag_two = true; // 把这个移到 if 判断的话，会有死锁的
        if(!shouldContinue()){
            cv_two.notify_one();
            break;
        }

        cout << name << "输出: " << counter++ << endl;

        cv_two.notify_one();
    }
}

void noTwo(){
    string name = "B线程";

    while( shouldContinue() ){
        unique_lock<mutex> lock(mtx);

        cv_two.wait(lock, []{return flag_two;});
        flag_two = false;
        flag_three = true;
        if(!shouldContinue()){
            cv_three.notify_one();
            break;
        }

        cout << name << "输出: " << counter++ << endl;

        cv_three.notify_one();
    }

}

void noThree(){
    string name = "C线程";

    while( shouldContinue() ){
        unique_lock<mutex> lock(mtx);

        cv_three.wait(lock, []{return flag_three;});
        flag_three = false;
        flag_one= true;
        if( !shouldContinue() ){
            cv_one.notify_one();
            break;
        }

        cout << name << "输出: " << counter++ << endl;

        cv_one.notify_one();
    }
}

int main(){
    cin.tie(nullptr);
    cout.tie(nullptr);

    cout << "打印程序开始" << endl;
    // flag_one = true; 
    flag_two = true;
    vector<thread> threads;
    threads.emplace_back(noOne);
    threads.emplace_back(noTwo);
    threads.emplace_back(noThree);

    for(auto& t : threads){
        t.join();
    }

    cout << "程序结束" << endl;
}