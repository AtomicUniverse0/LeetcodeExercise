/**
    原题链接：https://kamacoder.com/problempage.php?pid=1064
*/

/**
    题目描述: 给定一个字符串 s，它包含小写字母和数字字符，请编写一个函数，将字符串中的字母字符保持不变，
    而将每个数字字符替换为number。 
    例如，对于输入字符串 "a1b2c3"，函数应该将其转换为 "anumberbnumbercnumber"。
*/

/**
    要点：
    如果用辅助数组的话，没什么难的，问题在于如何在原数组上操作，而不是用额外的数组。
    另外，如何读取字符串呢？
*/

#include <iostream>
#include <string>

using namespace std;

int main(){
    string s;
    cout << "Please input a string: ";
    cin >> s;

    int oldSize = s.size();
    int numCount = 0;
    for(const char& c : s){
        if(isdigit(c)){
            ++numCount;
        }
    }


    int newSize = oldSize + numCount * 5;
    s.resize(newSize);

    int oldTail = oldSize - 1;
    int newTail = newSize - 1;

    while(oldTail < newTail){
        if(isdigit(s[oldTail])){
            // number
            s[newTail--] = 'r';
            s[newTail--] = 'e';
            s[newTail--] = 'b';
            s[newTail--] = 'm';
            s[newTail--] = 'u';
            s[newTail--] = 'n';
        }else{
            s[newTail--] = s[oldTail];
        }

        --oldTail;
    }

    cout << "Result is :" << endl;
    cout << s << endl;
}
