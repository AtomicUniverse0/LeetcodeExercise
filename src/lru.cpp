#include <iostream>
#include <map>
#include <cassert>
#include <stdexcept>
#include <string>

using namespace std;
/**
    原题链接：https://leetcode.cn/problems/3sum/description/
*/


/**
    要点：LRU 缓存，已经比较熟练了，要注意以下几点：

    Node 不要有key，还要有val，你说这是为啥?
    Node 干脆还是独立于LinkList比较好，这样写代码快一些
    LinkList 记得写析构函数，而且要虚的。
    LinkList 的接口有： 头插入，得到尾部，删除节点，移动到头部， 应该优化成: 得到尾部，删除节点，插入节点到指定节点后，移动到头部

*/

/**
    input格式: 先输入n, 然后输入n个操作，及其函数
*/

/**
    测试用例: 
    10
    "init" 2
    "put"  1 1 
    "put"  2 2
    "get"  1 
    "put"  3 3
    "get"  2 
    "put"  4 4 
    "get"  1 
    "get"  3
    "get"  4
*/


/**
    已通过校验
*/
class LinkList{
private:
    friend class LRUCache;
    
    class Node{
    public:
        Node() = default;
        Node(int key, int val) : key_(key), val_(val) {}

        int getKey() { return key_; }
        void setKey(int key) { this->key_ = key;}

        int getVal(){ return val_;}
        void setVal(int val) { this->val_ = val;}

        Node* getPrev() { return prev_;}
        void setPrev(Node* prev) {this->prev_ = prev;}

        Node* getNext() {return next_;}
        void setNext(Node* next) {this->next_ = next;}
    
    private:
        int key_;
        int val_;
        Node* prev_;
        Node* next_;
    };

    Node* dummyHead;
    Node* dummyTail;

public:
    LinkList(){
        dummyHead = new Node();
        dummyTail = new Node();
        dummyHead->setNext(dummyTail);
        dummyHead->setPrev(dummyTail);

        dummyTail->setPrev(dummyHead);
        dummyTail->setNext(dummyHead);
    }

    virtual ~LinkList(){
        while(!empty()){
            Node* tail = getTail();
            deleteNode(tail);
        }

        delete dummyHead;
        delete dummyTail;
    }

    void moveToHead(Node* node){
        Node* prev = node->getPrev();
        prev->setNext(node->getNext());
        node->getNext()->setPrev(prev);

        insertAtHead(node);
    }

    void deleteNode(Node* node){
        Node* prev = node->getPrev();
        prev->setNext(node->getNext());
        node->getNext()->setPrev(prev);

        delete node;
    }

    void insertAtHead(Node* node){
        node->setNext(dummyHead->getNext());
        node->getNext()->setPrev(node);
        node->setPrev(dummyHead);
        dummyHead->setNext(node);
    }

    Node* getTail(){
        return dummyTail->getPrev();
    }

    bool empty(){
        return dummyHead->getNext() == dummyTail;
    }
};

class LRUCache{
public:
    LRUCache(unsigned capcity) : capcity_(capcity) {
        assert(capcity_ > 0);
    }

    int get(int key);

    void put(int key, int value);

private:

    unsigned capcity_;
    LinkList linkList_;
    map<int, LinkList::Node*> keyNodeMap_;
};

int LRUCache::get(int key){
    if(keyNodeMap_.find(key) == keyNodeMap_.end()){
        return -1; // empty
    }

    LinkList::Node* target = keyNodeMap_[key];
    linkList_.moveToHead(target);
    return target->getVal();
}

void LRUCache::put(int key, int val){
    // 若存在，则替换 
    if(keyNodeMap_.find(key) != keyNodeMap_.end()){
        LinkList::Node* target = keyNodeMap_[key];
        target->setVal(val);
        linkList_.moveToHead(target);
        return;
    }

    // 若不存在，则插入,若满了，则驱逐
    if(keyNodeMap_.size() == capcity_){
        LinkList::Node* tail = linkList_.getTail();
        keyNodeMap_.erase(tail->getKey());
        linkList_.deleteNode(tail);
    }

    auto newNode = new LinkList::Node(key, val);
    linkList_.insertAtHead(newNode);
    keyNodeMap_[newNode->getKey()] = newNode;
}

int main(){
    string operation;

    unique_ptr<LRUCache> lruCachePtr;

    cout << "Please input the number of operation" << endl;
    int n ;
    cin >> n;

    while(n--){
        cout << "Please your operation and args: " << endl;
        cin >> operation;

        if(operation == "init"){
            assert(lruCachePtr.get() == nullptr);
            int capacit;
            cin >> capacit;
            lruCachePtr = make_unique<LRUCache>(capacit);
        }else if(operation == "get"){
            int key;
            cin >> key;
            cout << lruCachePtr->get(key) << endl;
        }else if(operation == "put"){
            int key, val;
            cin >> key >> val;
            lruCachePtr->put(key, val);
        }else {
            cout << "[Error]: invaild input: " + operation << endl;
        }
    }
}