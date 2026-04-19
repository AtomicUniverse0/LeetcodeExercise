/**
    https://leetcode.cn/problems/linked-list-cycle-ii/description/
*/


/**
    典中典，环形链表判断连接点，那个公式写出来的话就能秒，要熟练那个公式
*/

/**
    直接让对方输入每个节点的值，如果输入完毕，就输入 ctrl + D 来结束输入
    随后，再跟随一个整数，表示连接点的值，如果没有连接点，就输入 -1
*/

/**
    测试用例:期待输出
    1 1 -1 : -1
    3 2 0 -4 1 : 1
    3 2 0 -4 0 : 0
    3 2 0 -4 2 : 3
*/


#include <iostream>
#include <cassert>

using namespace std;

class Node{
public:
    Node() = default;

    void setVal(int val) {val_ = val;}
    int getVal() const { return val_;}

    void setNext(Node* next) {next_ = next;}
    Node* getNext() const {return next_;}

private:
    int val_;
    Node* next_;
};

/**
    从用户处读取环形链表
*/
Node* readInput(){

    cout << "PLease input your list node number: " << endl;
    int count;
    cin >> count;
    assert( count > 0);

    cout << "Reading..." << endl;

    Node* dummyHead = new Node();
    Node* tail = dummyHead;

    int val;
    int loop = count;
    while( loop-- > 0 ){
        cin >> val;
        Node* newNode = new Node();

        newNode->setVal(val);
        tail->setNext(newNode);
        tail = newNode;
        ++count;
    }
    cout << "Successed reead." << endl;

    cout << "Please input your pos" << endl;

    int pos;
    cin >> pos;
    if(pos == -1){
        return dummyHead;
    }
    assert(pos > -1 && pos < count);

    Node* loopPoint = dummyHead->getNext();
    while(pos-- > 0){
        loopPoint = loopPoint->getNext();
    }
    tail->setNext(loopPoint);

    return dummyHead;
}

/**
    假设输入的链表非空
*/
int findLoopPoint(Node* dummyHead){
    Node* fast = dummyHead->getNext(); // 
    Node* slow = dummyHead->getNext();
    /*
    x ,y, z -> 
    2(x + y)  = x + y + n * (y + z)
    -> x + y = n * (y + z) = (n - 1)(y + z) + y + z
    -> x = (n - 1) (y + z) + z
    */

    while(true){
        if(fast->getNext() == nullptr || fast->getNext()->getNext() == nullptr){
            return -1;
        }
        fast = fast->getNext()->getNext();
        slow = slow->getNext();
        if(fast == slow){
            break;
        }
    }

    // 运行到这里，说明链表一定有环，而且fast和slow正处于交点。 x = 2 z
    int pos = 0;
    fast = dummyHead->getNext();
    while(fast != slow){
        fast = fast->getNext();
        slow = slow->getNext();
        pos += 1;
    }

    return pos;
}

int main(){
    // 假设输入的链表总是非空的
    Node* dummyHead = readInput();

    cout << findLoopPoint(dummyHead) << endl;
}