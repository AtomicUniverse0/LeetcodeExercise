
/**
    原题链接：https://leetcode.cn/problems/repeated-substring-pattern/
*/

/**
给定一个非空的字符串 s ，检查是否可以通过由它的一个子串重复多次构成
*/

/**
    要点：
    暴力其实很简单，但我想到了一种空间换时间的办法
*/

/**
    测试用例: abab                  true
            aba                     true
            abcabcabcabc            true
            abaababaab              true
            abacababacab            true
            abcd                    false
*/
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const bool DEBUG_MODE = true;

template<typename ... ArgTypes>
void debugPrint(ArgTypes ... args){
    if(DEBUG_MODE){
        (cout << ... << args) << endl;
    }
}

vector<int> buildNext(const string& pattern){
    vector<int> next(pattern.size());

    int j = -1;
    next[0] = j;
    for(int i=1; i < pattern.size(); i++){
        while( j > -1 && pattern[j +1] != pattern[i]){
            j = next[j];
        }

        if(pattern[j+1] == pattern[i]){
            ++j;
        }

        next[i] = j;
    }

    return next;
}

bool hasRepeatedSubstr(const string& input){
    vector<int> next = buildNext(input);
    
    cout << "next: ";
    for(const int i : next){
        cout << i << " ";
    }
    cout << endl;

    size_t len = input.size() - (next[input.size() - 1] + 1);

    return  (input.size() > len )&& (input.size() % len == 0);
}

int main(){
    string input;
    cout << "please input your string: " << endl;
    cin >> input;

    cout << (hasRepeatedSubstr(input) ? "true" : "false") << endl;
     
}
