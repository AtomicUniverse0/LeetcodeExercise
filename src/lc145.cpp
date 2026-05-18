/**
    https://leetcode.cn/problems/binary-tree-preorder-traversal/description/
*/

/**
    二叉树的后序遍历
*/

/**
    输入一行数字，以空格分割。如果为空，则用#表示空节点
*/

/**
    测试用例:期待输出
    1,#,2,3 : 3 2 1
    1,2,3,4,5,#,8,#,#,6,7,9 : 4,6,7,5,2,9,8,3,1
    # : (空)
    1 : 1
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

class Node{
public:
    Node(int val) : val_(val), left_(nullptr), right_(nullptr) {}

    void setVal(int val){ val_ = val;}
    int  getVal() const {return val_;}

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
    string elem;
    vector<string> nodes;
    while(ss >> elem){
        nodes.push_back(elem);
    }

    if(nodes.empty() || nodes[0] == "#"){
        return nullptr;
    }

    Node* root = new Node( std::stoi( nodes[0] ) );
    queue<Node*> q;
    q.push(root);

    int i = 1;
    while(i < nodes.size() && !q.empty()){
        Node* current = q.front();
        q.pop();

        if(i < nodes.size() && nodes[i] != "#"){
            current->setLeft(new Node( std::stoi( nodes[i] ) ));
            q.push(current->getLeft());
        }
        ++i;

        if(i < nodes.size() && nodes[i] != "#"){
            current->setRight(new Node( std::stoi(nodes[i]) ));
            q.push(current->getRight());
        }
        ++i;
    }

    return  root;
}

void traversal(Node* root){
    if(root == nullptr){
        cout << "list is emtpy" << endl;
        return;
    }

    stack<Node*> stk;
    stk.push(root);
    vector<int> res;

    while(!stk.empty()){
        Node* current = stk.top();
        stk.pop();

        res.push_back(current->getVal());
        if(current->getLeft() != nullptr) {stk.push(current->getLeft());}
        if(current->getRight() != nullptr) {stk.push(current->getRight());}
    }

    std::reverse(res.begin(), res.end());

    for(const auto& elem : res){
        cout << elem << " ";
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
    std::getline(cin, line);

    Node* root = buildTree(line);

    traversal(root);

    deleteTree(root);
}