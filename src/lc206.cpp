#include <cstddef>
#include <iostream>

#include <iostream>

using namespace std;

/**
    原题链接: https://leetcode.cn/problems/reverse-linked-list/description/
*/


/**
    特别简单，重点是联系链表的输入输出和面向对象编程
*/

/**
    先输入n，然后输入几个数字作为节点。要求尾插法
*/

/**
    测试用例:
*/

class Node{
public:
    Node(int val) : val_(val), next_(nullptr) {}
    Node(int val, Node* next) : val_(val), next_(next) {}

    int getVal() {return val_;}
    Node* getNext() {return next_;}
    void setNext(Node* next) {next_ = next;}

private:
    int val_;
    Node* next_;
};

Node* readList(){
    int n; 
    cin >> n;

    Node* dummyHead = new Node(-1);
    Node* tail = dummyHead;

    int val;
    while(n-- > 0){
        cin >> val;
        Node* newNode = new Node(val);
        tail->setNext(newNode);
        tail = tail->getNext();
    }

    return dummyHead;
}

Node* reverseList(Node* dummyHead){
    if(dummyHead->getNext() == nullptr){
        return dummyHead; // 空链表
    }

    Node* last = nullptr;
    Node* current = dummyHead->getNext();

    while(current != nullptr){
        Node* temp = current->getNext();
        current->setNext(last);
        last = current;

        current = temp;
    }

    dummyHead->setNext(last);
    return dummyHead;
}

void printList(Node* dummyHead){
    Node* current = dummyHead->getNext();

    if(current == nullptr){
        cout << "empty List" << endl;
        return;
    }

    while(current != nullptr){
        cout << current->getVal() << " ";
        current = current->getNext();
    }
    cout << endl;
}

void releaseList(Node* dummyHead){
    Node* current = dummyHead;

    while(current != nullptr){
        Node* next = current->getNext();
        delete current;

        current = next;
    }
}

int main(){

    Node* dummyHead = readList();

    dummyHead = reverseList(dummyHead);

    printList(dummyHead);

    releaseList(dummyHead);
}