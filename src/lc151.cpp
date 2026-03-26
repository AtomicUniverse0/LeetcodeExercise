/**
    原题链接：https://leetcode.cn/problems/reverse-words-in-a-string/
*/

/**
    要点：反转字符串,用辅助空间的话很简单,能否不使用额外的空间做完这题呢
    C++字符串能否删除最前面的成员？ -- 不能
    C++字符串，有个substr函数，它会触发拷贝吗？ -- 会

    挑战：用O（1）空间复杂度的算法来完成这道题
    删除空格的操作还不熟练，要多写几次
*/

/**
    测试用例:
    输入： "the sky is blue" 输出："blue is sky the"
    输入： "  hello world  " 输出；"world hello"
    输入： "a good   example" 输出： "example good a"
*/

#include <algorithm>
#include <iostream>
#include <string>
#include <sys/resource.h>

using namespace std;

/**
    删除字符串两端的空格，并且令内部相邻两词之间的空格为1
*/
void deleteRedundantSpace(string& str){
    int fast = 0;
    int slow = 0;

    while(fast < str.size() && str[fast] == ' '){
        ++fast;
    }
    if(fast >= str.size()) { str = ""; return;}

    while(fast < str.size()){
        if(slow > 0) { // 说明不是第一个词，那么词与词之间应该有空格
            str[slow++] = ' '; 
        }

        while(fast < str.size() && str[fast] != ' '){
            str[slow++] = str[fast++];
        }
        while(fast < str.size() && str[fast] == ' '){
            ++fast;
        }
    }

    str.resize(slow);
}

/**
    将 input[left, right] 反转
*/
void reverseSubstr(string& input, unsigned left,unsigned right){
    while(left < right){
        std::swap(input[left], input[right]);
        ++left;
        --right;
    }
}

void doReverse(string& input){
    if(input.empty()){ return;}

    reverseSubstr(input, 0, input.size() - 1); 
    int left = 0;
    int right = 0;
    while(left < input.size()){
        while(right < input.size() && input[right] != ' '){
            ++right;
        }

        reverseSubstr(input, left, right - 1);
        left = right + 1;
        right = left + 1;
    }
}

int main(){
    string input;
    cout << "Please input your sentence" << endl;
    std::getline(cin, input);

    deleteRedundantSpace(input);
    if(input.empty()){
        cout << "Error, input is empty" << endl;
        return -1;
    }

    doReverse(input);

    cout << "Output is : " << endl;
    cout << input << endl;
}
//     读取不能用cin, 它遇到空格会停止读取, 要用getline!