#include <iostream>

using namespace std;

// https://leetcode.cn/problems/remove-linked-list-elements/submissions/662798137/

/**
    移除链表，本质上很简单，但是写的不熟练，包括 链表创建，删除时使用delete等
*/

/**
    input格式: 先输入val, 然后输入n, 然后输入n个整数，代表链表的值
*/

/**
    测试用例:
    1. 6 7 1 2 6 3 4 5 6    期待结果: [1,2,3,4,5]
    2. 1 0                  期待结果: 空链表
    3. 7 4 7 7 7 7          期待结果: 空链表
*/

class ListNode{
    public: 
    int val;
    ListNode* next;

    ListNode(int val){
        this->val = val;
        this->next = nullptr;
    }
};

/*
    尾插法读入链表
    链表具有一个虚拟头节点
*/
ListNode* ReadList(int n){
    ListNode* dummyHead = new ListNode(-1);
    ListNode* tail = dummyHead;

    while(n--){
        int val;
        cin >> val;
        ListNode* newNode = new ListNode(val);
        tail->next = newNode;
        tail = tail->next;
    }
    
    return dummyHead;
}   
    
void PrintList(ListNode* head){
    if(head == nullptr){
        cout << "Empyt List" << endl;
        return;
    }

    while(head != nullptr){
        cout << head->val << " ";
        head = head->next;
    }
    cout << endl;
}

int main(){
    int n, target;
    cin >> target >> n;
    
    ListNode* dummyHead = ReadList(n);

    ListNode* current = dummyHead;
    ListNode* next    = dummyHead->next;

    while(next != nullptr){
        if(next->val == target){
            ListNode* temp = next;

            next = next->next;
            current->next = next;

            delete temp;
        }else{
            current = current->next;
            next = next->next;
        }
    }

    ListNode* head = dummyHead->next;
    delete  dummyHead;
    PrintList(head);
}