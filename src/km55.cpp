/**
    原题链接：https://kamacoder.com/problempage.php?pid=1065
*/

/**
题目描述：
把字符串尾部的若干个字符转移到字符串的前面。给定一个字符串 s 和一个正整数 k，
请编写一个函数，将字符串中的后面 k 个字符移到字符串的前面
例如，对于输入字符串 "abcdefg" 和整数 2，函数应该将其转换为 "fgabcde"

如果你能想明白思路，就不必写了，很简单的
*/

/**
输入样例：k, string
*/

/**
    要点：
    不使用辅助空间，实现这道题
*/
#include <iostream>
#include <string>

using namespace std;

void ReverseSubStr(string& str, int head, int tail){
    if(head > tail){
        cout << "Error, head " << head << "larget than tail " << tail << endl;
        std::abort();
    }
    while(head < tail){
        std::swap(str[head], str[tail]);
        ++head;
        --tail;
    }
}

int main(){
    int k;
    string input;
    cout << "Please input k: ";
    cin >> k;
    cout << endl << "Please input str: ";
    cin >> input;
    cout << endl;

    k = k % input.size();

    // abcdefg 2 ，，， 看起来是两次反转
    ReverseSubStr(input, 0, input.size() - 1);
    ReverseSubStr(input, 0, k - 1);
    ReverseSubStr(input, k , input.size() - 1);

    cout << input << endl;
}