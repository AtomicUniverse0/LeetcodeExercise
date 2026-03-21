#include <iostream>
#include <vector>

using namespace std;

vector<int> buildNext(const string& pattern){
    vector<int> next(pattern.size());

    int j = 0;
    next[0] = j; // next[i] 的意思是，如果pattern[i]不匹配，那么需要用 next[i] 来匹配？

    for(int i=1; i < pattern.size(); ++i){
        while( j > 0 && pattern[i] != pattern[j]){
//            cout << j << " 回退 " << next[j] << endl;
            j = next[j - 1];
        }

        if(pattern[i] == pattern[j]){
            ++j;
        }

        next[i] = j;
    }

    return next;
}

int kmp(const string& text, const string& pattern){
    vector<int> next = buildNext(pattern);

    int tp = 0; // text pointer
    int pp = 0; // pattern pointer

    for(; tp < text.size(); ++tp){
        while(pp > 0 && text[tp] != pattern[pp]){
//            cout << pp << " 回退 " << next[pp] << endl;
            pp = next[pp-1];
        }

        if(text[tp] == pattern[pp]){
            ++pp;
        }

        if(pp == pattern.size()){
            return tp - pattern.size() + 1;
        }
    }

    return -1;
}

int main(){
    string pattern;
    string text;

    cout << "Please input your text:" << endl;
    cin >> text;
    cout << "Please input your pattern" << endl;
    cin >> pattern;

    cout << kmp(text, pattern) << endl;

    return 0;
}