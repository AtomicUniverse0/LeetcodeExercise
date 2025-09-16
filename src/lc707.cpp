#include <iostream>
#include <cassert>

using namespace std;

/**
    原题链接: https://leetcode.cn/problems/design-linked-list/

实现 MyLinkedList 类：

MyLinkedList() 初始化 MyLinkedList 对象。
int get(int index) 获取链表中下标为 index 的节点的值。如果下标无效，则返回 -1 。
void addAtHead(int val) 将一个值为 val 的节点插入到链表中第一个元素之前。在插入完成后，新节点会成为链表的第一个节点。
void addAtTail(int val) 将一个值为 val 的节点追加到链表中作为链表的最后一个元素。
void addAtIndex(int index, int val) 将一个值为 val 的节点插入到链表中下标为 index 的节点之前。如果 index 等于链表的长度，那么该节点会被追加到链表的末尾。如果 index 比长度更大，该节点将 不会插入 到链表中。
void deleteAtIndex(int index) 如果下标有效，则删除链表中下标为 index 的节点。
*/


/**
    要点：模拟链表的多个方法，总是写不对。。。
    难点在于，要维护一个tail节点。 实际上，愿意牺牲时间复杂度，可以不用tail，这样的话，心智负担会小很多
*/

/**
    input格式: 先输入n,表示有n个操作。然后每行输入字符串和对应的参数
*/

/**
    测试用例:
    1. 
7
MyLinkedList
addAtHead 1
addAtTail 3
addAtIndex 1 2
get 1
deleteAtIndex 1
get 1

    2.

*/

const bool DEBUG = false;

class MyLinkedList {
private:
    int n = 0; // 链表节点数量

    struct Node{
        int val;
        Node* next;

        Node(int value){
            this->val = value;
            this->next = nullptr;
        }
    };

    Node* dummyHead;
    Node* tail;

    bool IsValidIndex(int index){
        return index >= 0 && index < n;
    }

public:
    MyLinkedList() {
        this->n = 0;
        this->dummyHead = new Node(0x3f3f3f3f);
        tail = this->dummyHead;
    }

    ~MyLinkedList(){
        delete this->dummyHead;
    }

    void DebugPrint(){
        if(!DEBUG) return;

        Node* cur = dummyHead->next;

        while(cur != nullptr){
            cout << cur->val << " ";
            cur = cur->next;
        }
        cout << endl;
    }
    
    int get(int index) {
        if(!IsValidIndex(index)) {return -1;}

        Node* cur = dummyHead;
        while(index--){
            cur = cur->next;
        }

        return cur->next->val;
    }
    
    void addAtHead(int val) {
        auto newNode = new Node(val);
        newNode->next = dummyHead->next;
        dummyHead->next = newNode;

        if(dummyHead == tail){
            tail = newNode;
        }

        ++n;
    }
    
    void addAtTail(int val) {
        auto newNode = new Node(val);
        tail->next = newNode;
        tail = newNode;

        ++n;
    }
    
    void addAtIndex(int index, int val) {
        assert(index >= 0);

        if(index > n) {return;}
        if(index == n){ return addAtTail(val); }

        Node* cur = dummyHead;
        while(index--){
            cur = cur->next;
        }

        auto newNode = new Node(val);
        newNode->next = cur->next;
        cur->next = newNode;

        if(cur == tail){
            tail = newNode;
        }

        ++n;
    }
    
    void deleteAtIndex(int index) {
        if(!IsValidIndex(index)) {return;}

        Node* cur = this->dummyHead;

        while(index--){
            cur = cur->next;
        }

        Node* temp = cur->next;
        cur->next = cur->next->next;
        if(temp == tail){
            tail = cur;
        }
        delete temp;
        --n;
    }
};


int main(){
    int n;
    int index;
    int value;
    string funcname;

    MyLinkedList* obj = nullptr;

    cin >> n;
    while(n--){
        cin >> funcname;
        
        if(funcname == "MyLinkedList"){
            obj = new MyLinkedList();
        }else if(funcname == "get"){
            cin >> index;
            cout << obj->get(index) << endl;
        }else if(funcname == "addAtHead"){
            cin >> value;
            obj->addAtHead(value);
            obj->DebugPrint();
        }else if(funcname == "addAtTail"){
            cin >> value;
            obj->addAtTail(value);
            obj->DebugPrint();
        }else if(funcname == "addAtIndex"){
            cin >> index >> value;
            obj->addAtIndex(index, value);
            obj->DebugPrint();
        }else if(funcname == "deleteAtIndex"){
            cin >> index;
            obj->deleteAtIndex(index);
            obj->DebugPrint();
        }else{
            assert(0);
        }
    }
}