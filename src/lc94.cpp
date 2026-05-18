/**
    https://leetcode.cn/problems/binary-tree-inorder-traversal/description/
*/

/**
    二叉树的中序遍历，递归版超简单，重点写迭代版，以及建树流程

    迭代版还不熟悉，可以多做
*/

/**
    输入一行数字，以空格分割。如果为空，则用#表示空节点
*/

/**
    测试用例:期待输出
    1,#,2,3 :  1,3,2
    1,2,3,4,5,#,8,#,#,6,7,9 : 4,2,6,5,7,1,3,9,8
    # : (空)
    1 : 1
*/

#include <cstddef>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <stack>

using namespace std;

class Node{
public:
    Node(int val) : val_(val), left_(nullptr), right_(nullptr){}

    void setVal(int val) {val_ = val;}
    int getVal() const {return val_;}

    void setLeft(Node* left) {left_ = left;}
    Node* getLeft() const {return left_;}

    void setRight(Node* right) {right_ = right;}
    Node* getRight() const {return right_;}
private:
    int val_;
    Node* left_;
    Node* right_;
};

Node* buildTree(const string& line){

    stringstream ss(line);
    vector<string> nodes;
    string elem;
    while(ss >> elem){
        nodes.push_back(elem);
    }

    if(nodes.empty() || nodes[0] == "#"){
        return nullptr;
    }

    queue<Node*> q;
    Node* root = new Node( std::stoi(nodes[0]) );
    q.push(root);

    int i = 1;
    while(!q.empty()){
        Node* current = q.front();
        q.pop();

        if(i < nodes.size() && nodes[i] != "#"){
            current->setLeft(new Node( std::stoi(nodes[i]) ));
            q.push( current->getLeft() );
        }
        ++i;

        if(i < nodes.size() && nodes[i] != "#"){
            current->setRight(new Node(std::stoi(nodes[i])));
            q.push(current->getRight());
        }
        ++i;
    }

    return root;
}

void traverse(Node* root){
    if(root == nullptr ) {
        cout << "tree is empty" << endl;
        return;
    }

    Node* cur = root;
    stack<Node*> stk;
    while( cur != nullptr || !stk.empty()){
        if(cur != nullptr){
            stk.push(cur);
            cur = cur->getLeft();
        }else{
            cur = stk.top();
            stk.pop();
            cout << cur->getVal() << " ";

            cur = cur->getRight();
        }
    }
}

void deleteTree(Node* root){
    if(root == nullptr){return;}
    deleteTree(root->getLeft());
    deleteTree(root->getRight());
    delete root;
}

int main(){
    string line;
    std::getline(cin, line);

    Node* root = buildTree(line);

    traverse(root);

    deleteTree(root);
}