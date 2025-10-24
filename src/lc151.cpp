/**
    原题链接：https://leetcode.cn/problems/reverse-words-in-a-string/
*/

/**
    要点：反转字符串,用辅助空间的话很简单,能否不使用额外的空间做完这题呢
    C++字符串能否删除最前面的成员？ -- 不能
    C++字符串，有个substr函数，它会触发拷贝吗？ -- 会

    读取不能用cin, 它遇到空格会停止读取, 要用getline!
    删除内部空格的操作还不熟练，你要多写几次
    我的删除空格逻辑太冗余了，可以参考代码随想录的最优解法
*/

/**
    测试用例:
    输入： "the sky is blue" 输出："blue is sky the"
    输入： "  hello world  " 输出；"world hello"
    输入： "a good   example" 输出： "example good a"
*/

#include <iostream>
#include <string>
#include <string_view>

using namespace std;

// 反转 str[begin], str[end] 之间的所有字符串
static inline void ReverseSubStr(string& str, int begin, int end){
    while(begin < end){
        char tmp = str[begin];
        str[begin] = str[end];
        str[end] = tmp;

        ++begin;
        --end;
    }
}

int main(){
    // string似乎没有类似于 pop_front()的接口，但erase的复杂度又是O(n)
    // 只好用原始的办法了
    string input;
    cout << "Please input your string input:";
    std::getline(cin, input);

    // 删除两侧的空格
    int begin = 0;
    int end = input.size() - 1;

    while(begin <= end && input[begin] == ' '){
       ++begin; 
    }

    while(begin <= end && input[end] == ' '){
        --end;
    }

    if(begin > end){
        cout << "Error, input is empty string" << endl;
        return 0;
    }


    // 删除字符串内的空格
    int redundantSpaceCount = 0;
    for(int i=begin; i <= end; ++i){
        while(input[i] == ' ' && input[i+1] == ' '){
            ++i;
            ++redundantSpaceCount;
        }
    }

    int newEnd = end - redundantSpaceCount;
    int i = newEnd; // 将j地址的字符拷贝到i的地址
    int j = end; 


    // 有坑！ 不能写成 while (i < j), 因为i, j 共同修改同一个数组
    while( j > newEnd){
        if(input[j] != ' '){
            input[i] = input[j--];
        }else{
            input[i] = ' ';
            while(input[j] == ' '){
                --j;
            }
            cout << endl;
        }

        --i;
        if(i < 0){
            std::abort();
        }
    }
    end = newEnd;

    ReverseSubStr(input, begin, end);
    for(int ssb = begin; ssb <= end; ssb ++){ // ssb: SubStringBegin
        int sse = ssb; // sse: SubStringEnd
        while(sse + 1 <= end && input[sse + 1] != ' '){
            ++sse;
        }

        ReverseSubStr(input, ssb, sse);

        ssb = sse + 1; // 指向空格，再递增一次，就会指向下一个字串的开头
    }

    cout << string_view(input).substr(begin, end - begin + 1) << endl;
    return 0;
}