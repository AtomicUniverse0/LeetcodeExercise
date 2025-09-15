#include <iostream>

using namespace std;

/**
    原题链接: https://leetcode.cn/problems/remove-linked-list-elements/submissions/662798137/
*/


/**
    要点：
    移除链表，本质上很简单，但是写的不熟练，包括: 
        1. 链表创建，打印
        2. 删除时使用delete
        3. 要注意，只用cur节点就好，不必分cur和next，这样可以减少心智负担
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

class Node{
    public:
    int val;
    Node* next;

    Node(int val){
        this->val = val;
    }
};

// 读入链表，带虚拟头节点
Node* ReadList(int n){
    Node* dummyHead = new Node(-1);
    Node* tail = dummyHead;

    while(n--){
        int val;
        cin >> val;

        Node* newNode = new Node(val);

        tail->next = newNode;
        tail = tail->next;
    }

    return dummyHead;
}

void PrintList(Node* head){
    if(head == nullptr){
        cout << "empty list" << endl;
    }

    while(head != nullptr){
        cout << head->val << " ";
        head = head->next;
    }
}

int main(){
    int target, n;
    cin >> target >> n;

    Node* dummyHead = ReadList(n);

    Node* cur = dummyHead;

    while(cur->next != nullptr){
        if(cur->next->val == target){
            Node* temp = cur->next;
            cur->next = cur->next->next;
            delete temp;
        }else{
            cur = cur->next;
        }
    }

    Node* head = dummyHead->next;
    delete dummyHead;
    PrintList(head);
}