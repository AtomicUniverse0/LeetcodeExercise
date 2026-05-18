/**
    https://leetcode.cn/problems/binary-tree-preorder-traversal/description/
*/


/**
    二叉树的先序遍历，递归版超简单，重点写迭代版，以及建树流程
*/

/**
    输入一行数字，以空格分割。如果为空，则用#表示空节点
*/

/**
    测试用例:期待输出
    1,#,2,3 : 1 2 3
    1,2,3,4,5,#,8,#,#,6,7,9 : 1 2 4 5 6 7 3 8 9
    # : (空)
    1 : 1
*/

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

class Node{
public:
    Node(int val) : val_(val), left_(nullptr), right_(nullptr){}

    void setVal(int val) {val_ = val;}
    int getVal() const { return val_;}

    void setLeft(Node* left) {this->left_ = left;}
    Node* getLeft() const {return left_; }

    void setRight(Node* right) {this->right_ = right;}
    Node* getRight() const {return right_;}

private:
    int val_;
    Node* left_;
    Node* right_;
};

// 这段要求写成肌肉记忆
Node* buildTree(const string& line){
    stringstream ss(line);

    string val;
    vector<string> nodes;
    while(ss >> val){
        nodes.push_back(val);
    }

    if(nodes.empty() || nodes[0] == "#"){
        return nullptr;
    }

    queue<Node*> q;
    Node* root = new Node( std::stoi(nodes[0]));
    q.push(root);

    int i = 1;
    while(!q.empty() && i < nodes.size()){
        Node* current = q.front();
        q.pop();

        if(i < nodes.size() && nodes[i] != "#"){
            Node* left = new Node(std::stoi(nodes[i]));
            current->setLeft(left);
            q.push(left);
        }
        ++i;

        if(i < nodes.size() && nodes[i] != "#"){
            Node* right = new Node(std::stoi(nodes[i]));
            current->setRight(right);
            q.push(right);
        }
        ++i;
    }

    return root;
}


void loopTraverse(Node* root){
    if(root == nullptr){
        cout << "The tree is empty" << endl;
        return;
    }

    stack<Node*> stk;
    stk.push(root);

    while( ! stk.empty() ){
        Node* current = stk.top(); 
        stk.pop();

        cout << current->getVal() << " ";

        if(current->getRight() != nullptr){
            stk.push(current->getRight());
        }

        if(current->getLeft() != nullptr){
            stk.push(current->getLeft());
        }
    }
}

void deleteTree(Node* root){
    if(root == nullptr){
        return;
    }
    deleteTree(root->getLeft());
    deleteTree(root->getRight());
    delete root;
}


int main(){
    string line;
    std::getline(cin , line);

    Node* root = buildTree(line);

    loopTraverse(root);

    deleteTree(root);

    return 0;
}