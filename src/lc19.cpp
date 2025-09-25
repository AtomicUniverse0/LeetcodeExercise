#include <iostream>
#include <cassert>

using namespace std;

/**
    原题链接: https://leetcode.cn/problems/remove-nth-node-from-end-of-list/description/
    下面这些对
*/


/**
    要点：
    移除链表倒数第n个节点，思路好想，但是写的不熟练，主要体现在:
        1. 链表创建，打印
        2. 删除时使用delete
        3. fast和slow，fast是null时，slow应该时多少？
*/

/**
    input格式: 先输入n 和 k, k表示要删除倒数第k个节点。 然后输入n个整数，代表链表的值 
*/

/**
    测试用例:
    1. 5 2 1 2 3 4 5   期待结果: [1,2,3,5]
    2. 1 1 1           期待结果: 空链表
    3. 2 2 1 2          期待结果: [1]
*/

struct Node{
private:
    int val;
    Node* next;

public:
    Node(int value){
        this->val = value;
        this->next = nullptr;
    }

    int GetVal(){
        return this->val;
    }

    void SetNext(Node* next){
        this->next = next;
    }

    Node* GetNext(){
        return this->next;
    }
};

Node* ReadList(int n){
    assert(n > 0);
    Node* dummyHead = new Node(-1);
    Node* tail = dummyHead;

    int val;
    while(n--){
        cin >> val;
        auto newNode = new Node(val);
        tail->SetNext(newNode);
        tail = tail->GetNext();
    }

    return dummyHead;
}

void PrintList(Node* head){
    if(head == nullptr){
        cout << "Empty List" << endl;
        return;
    }

    while(head != nullptr){
        cout << head->GetVal() << " ";
        head = head->GetNext();
    }
    cout << endl;
}

int n, k;

int main(){

    cout << " pleas input n and k: ";
    cin >> n >> k;
    cout << endl;

    if(k > n){
        PrintList(nullptr);
        return 0;
    }

    cout << "please input your List: ";

    Node* dummyHead = ReadList(n);
    
    Node* fast = dummyHead->GetNext();
    Node* slow = dummyHead;

    while(k--){
        fast = fast->GetNext();
    } 

    while(fast != nullptr){
        fast = fast->GetNext();
        slow = slow->GetNext();
    }

    
    Node* toBeDeleted = slow->GetNext();
    slow->SetNext(slow->GetNext()->GetNext());
    
    Node* head = dummyHead->GetNext();
    if(head == toBeDeleted){
        head = head->GetNext();
    }

    delete toBeDeleted;
    delete dummyHead;
    
    PrintList(head);
}