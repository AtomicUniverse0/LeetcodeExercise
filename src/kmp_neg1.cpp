/**
    原题链接：https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/
*/

/**
题目描述：
    典中典，写一次忘一次，希望能记住吧。。。启始下标是-1的版本
*/

/**
输入样例：
    text = "sadbutsad", pattern = "sad"  期待结果: 0
    text = "leetcode",  pattern = "leeto"  期待结果: -1
*/
#include <iostream>
#include <vector>

using namespace std;

vector<int> buildNext(const string& pattern){
    vector<int> next(pattern.size());

    int j = -1;
    next[0] = j; 

    for(int i=1; i<pattern.size(); i++){
        while(j > -1 && pattern[j+1] != pattern[i]){
            j = next[j];
        }

        if(pattern[j+1] == pattern[i]){
            j++;
        }

        next[i] = j;
    }

    return next;
}

int kmp(const string& pattern, const string& text){
    vector<int> next = buildNext(pattern);
    int tp = 0; // text pointer
    int pp = -1; // pattern pointer

    for( ; tp < text.size(); ++tp){
        while( pp > -1 && text[tp] != pattern[pp + 1]){
            pp = next[pp];
        }
        
        if(text[tp] == pattern[pp + 1]){
            ++pp;
        }

        if(pp == pattern.size() - 1){
            return tp - pattern.size() + 1;
        }
        
    }
    return -1;
}

int main(){
    string text;
    string pattern;
    cout << "Please input your text:" << endl;
    cin >> text;
    cout << "Please input your pattern: " << endl;
    cin >> pattern;

    cout << kmp(pattern, text) << endl;

    return 0;
}