/**
    https://leetcode.cn/problems/construct-binary-tree-from-inorder-and-postorder-traversal/description/
*/

/**
    经典题目，通过中序与后序遍历构造二叉树
*/

/**
    输入格式: 先输入节点个数 n 然后输入 n 个 inorder，然后输入n个 postorder
*/

/**
    测试用例:期待输出
    5  9 3 15 20 7 9 15 7 20 3 : [3,9,20,null,null,15,7]
    1 -1 -1 : [-1]
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <string>
#include <queue>

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::queue;
using std::endl;


template<typename T>
class Node{

private:
    T val_;
    Node* left_;
    Node* right_;

public:
    Node(T val): val_(val), left_(nullptr), right_(nullptr){}

    const T& getVal(){ return val_;}

    void setLeft(Node* left) {left_ = left;}
    Node* getLeft() {return left_;}

    void setRight(Node* right) {right_ = right;}
    Node* getRight(){ return right_; }
};

/**  
    通过层序遍历输出树的内容
    注意这里我用一个vector来存字符串，这是为了除掉末尾的null
*/
template <typename T>
void printTree(Node<T>* root){
    if( root == nullptr){
        cout << "null" << endl;
        return;
    }

    vector<string> result;
    queue<Node<T>*> q;
    q.push(root);

    bool should_stop = false;
    uint32_t last_layer_size = 0;
    while( not should_stop){
        uint32_t n = q.size();
        last_layer_size = n;
        should_stop = true;
        while(n--){
            Node<T>* cur = q.front();
            q.pop();

            if(cur == nullptr){
                result.emplace_back("null");
                q.push(nullptr);
                q.push(nullptr);
            }else{
                should_stop = false;
                result.emplace_back(std::to_string(cur->getVal()));
                q.push( cur->getLeft() );
                q.push( cur->getRight());
            }
        }
    }

    while(last_layer_size--){
        result.pop_back();
    } 
    
    for(const auto& str : result){
        cout << str << " ";
    }
    cout << endl;
}

template<typename T>
Node<T>* buildTree(
    typename vector<T>::const_iterator inorder_begin, 
    typename vector<T>::const_iterator inorder_end,
    typename vector<T>::const_iterator postorder_begin, 
    typename vector<T>::const_iterator postorder_end
){
    if( inorder_begin >= inorder_end){
        return nullptr;
    }else if(inorder_begin + 1 == inorder_end){
        return new Node<T>( *inorder_begin );
    }

    // 从后序中找到根节点，然后用它来切割中序
    const T& root_val = *(postorder_end - 1);
    auto root_iter = std::find( inorder_begin , inorder_end , root_val );

    Node<T>* root = new Node<T>(root_val);

    // [inorder_left, root_idx - 1] 是左子树， [root_idx + 1, inorder_right] 是右子树
    uint32_t left_child_count = root_iter - inorder_begin;
    // [postorder_left, postorder_left + left_child_count] 是后序遍历中的左子树
    root->setLeft(
        buildTree<T>(
            inorder_begin, 
            root_iter, 
            postorder_begin,  
            postorder_begin + left_child_count
        )
    );

    root->setRight(
        buildTree<T>(
            root_iter + 1,  
            inorder_end, 
            postorder_begin + left_child_count + 1,   
            postorder_end - 1)
    );

    return root;
}

int main(){
    cout << "please input your values: ";

    int n = 0;
    cin >> n;
    
    assert( n > 0);
    vector<int> inorder(n);
    vector<int> postorder(n);
    
    for(int i=0; i < n; ++i){
        cin >> inorder[i];
    }
    for(int i=0; i < n; ++i){
        cin >> postorder[i];
    }

    auto root = buildTree<int>(
        inorder.cbegin(),
        inorder.cend(),
        postorder.cbegin(),
        postorder.cend()
    );

    printTree(root);
}