#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <sys/wait.h>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>

using std::unordered_map;
using std::cin;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::string;
/**
    原题链接：https://leetcode.cn/problems/3sum/description/
*/


/**
    要点：LRU 缓存，这里尝试使用模板来写出比较优雅的代码

    Node 干脆还是独立于LinkList比较好，这样写代码快一些
    可以让LRUCache 的 map 使用 unique_ptr<> 来管理Node的生命周期，这样就可以不写LinkList的析构函数了
    LinkList 的接口有： 头插入，得到尾部，删除节点，移动到头部， 应该优化成: 得到尾部，删除节点，插入节点到指定节点后，移动到头部
    试试写成模板格式

*/

/**
    input格式: 先输入n, 然后输入n个操作，及其函数
*/

/**
    测试用例: 
    10
    init 2
    put  1 1 
    put  2 2
    get  1 
    put  3 3
    get  2 
    put  4 4 
    get  1 
    get  3
    get  4
*/

template<class K, class V>
class Node{
public:
    Node() = default;
    Node(K key, V value){
        key_ = key;
        value_ = value;
        prev_ = nullptr;
        next_ = nullptr;
    }

    template<class T>
    void setKey(T&& key){
        key_ = std::forward<T>(key);
    }

    K& getKey(){
        return key_;
    }

    template<class T>
    void setValue(T&& value){
        value_ = std::forward<T>(value);
    }

    V& getValue(){
        return value_;
    }

    void setPrev(Node<K,V>* prev){
        prev_ = prev;
    }
    Node<K,V>* getPrev(){
        return prev_;
    }

    void setNext(Node<K,V>* next){
        next_ = next;
    }
    Node<K,V>* getNext(){
        return next_;
    }

private:
    K key_;
    V value_;
    Node<K,V>* prev_;
    Node<K,V>* next_;
};



template<class K, class V>
class LinkList{
public:
    LinkList(const size_t capacity): size_(0), capacity_(capacity){
        assert(capacity_ > 0);
        dummyHead_.setPrev(&dummyTail_);
        dummyHead_.setNext(&dummyTail_);
        dummyTail_.setPrev(&dummyHead_);
        dummyTail_.setNext(&dummyHead_);
    }

    bool full(){
        assert(size_ <= capacity_);
        return capacity_ == size_;
    }

    void moveToHead(Node<K,V>* nodePtr){
        deleteNode(nodePtr);
        addToHead(nodePtr);
    }

    void addToHead(Node<K, V>* nodePtr){
        ++size_;
        nodePtr->setNext(dummyHead_.getNext());
        nodePtr->setPrev(&dummyHead_);
        dummyHead_.getNext()->setPrev(nodePtr);
        dummyHead_.setNext(nodePtr);
    }

    Node<K,V>* getTail(){
        return dummyTail_.getPrev();
    }

    void deleteNode(Node<K, V>* nodePtr){
        assert( size_ > 0);
        --size_;

        auto next = nodePtr->getNext();
        auto prev = nodePtr->getPrev();
        next->setPrev(prev);
        prev->setNext(next);
    }

private:
    Node<K,V> dummyHead_;
    Node<K,V> dummyTail_;
    size_t size_;
    const size_t capacity_ ;
};

class LRUCache{
public:
    LRUCache(const size_t capacity):  list_(LinkList<int, int>(capacity)){};

    int get(int key);

    void put(int key, int value);

private:
    // 链表节点的生命周期由 hashMap_ 管理，链表自身只负责排序，不管理节点的声明周期。
    LinkList<int, int> list_;
    unordered_map<int, unique_ptr<Node<int, int>> > hashMap_;
};

int LRUCache::get(int key){
    if(hashMap_.find(key) == hashMap_.end()){
        return -1;
    }

    auto nodePtr = hashMap_[key].get();
    list_.moveToHead(nodePtr);
    return nodePtr->getValue();
}

void LRUCache::put(int key, int value){
    if(hashMap_.find(key) != hashMap_.end()){
        throw std::runtime_error("duplicate key!");
    }

    if(list_.full()){
        auto tail = list_.getTail();
        list_.deleteNode(tail);
        hashMap_.erase(tail->getKey());
    }

    auto newNode = std::make_unique<Node<int ,int>>(key, value);
    list_.addToHead(newNode.get());
    hashMap_[key] = std::move(newNode);
}

int main(){
    int n;
    string operation;    
    int key;
    int value;

    unique_ptr<LRUCache> lruCachePtr;
    
    cout << "please input your operation number" << endl;
    cin >> n;
    assert(n > 0);
    while(n--){
        cout << "please input your operation and args" << endl;
        cin >> operation;

        if(operation == "init"){
            assert(lruCachePtr.get() == nullptr);
            int capacity = -1;
            cin >> capacity;
            lruCachePtr = std::make_unique<LRUCache>(capacity);
        }else if(operation == "get"){
            cin >> key;
            int value = lruCachePtr->get(key);
            if(value == -1){
                cout << "no elem" << endl;
            }else{
                cout << value << endl;
            }
        }else if(operation == "put"){
            cin >> key >> value;
            lruCachePtr->put(key, value);
        }else{
            throw std::runtime_error("invalid operation: " + operation);
        }
    }
}