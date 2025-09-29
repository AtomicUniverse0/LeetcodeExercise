#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
    原题链接：https://leetcode.cn/problems/3sum/description/
*/


/**
    要点：经典三数之和，常看常新
*/

/**
    input格式: 先输入n, 然后输入n个整数，数组的值
*/

/**
    测试用例:
    1. 6 -1,0,1,2,-1,-4     期待结果: [[-1,-1,2],[-1,0,1]]
    2. 0,1,1                期待结果: 空链表
    3. 0,0,0                期待结果: [0, 0, 0]
*/

vector< vector<int> > ThreeNumSum(vector<int>& nums){
    vector<vector<int> > result;

    std::sort(nums.begin(), nums.end());

    int numSize = nums.size();
    for(int i=0; i < numSize - 2; i++){
        int target = -nums[i];
        int j = i + 1;
        int k = numSize - 1;

        while( j < k){
            int sum = nums[j] + nums[k];
            if( sum == target){
                result.push_back({ nums[i], nums[j], nums[k] });
                while( j < k && nums[j + 1] == nums[j]){
                    ++j;
                }
                ++j;

                while( j < k && nums[k - 1] == nums[k]){
                    --k;
                }
                --k;
            }else if(sum > target){
                while( j < k && nums[k - 1] == nums[k]){
                    --k;
                }
                --k;
            }else{
                while( j < k && nums[j + 1] == nums[j] ){
                    ++j;
                }
                ++j;
            }
        }

        while(i < numSize-2 && nums[i] == nums[i+1]){
            ++i;
        }
    }

    return result;
}

int main(){
    int n;
    cin >> n;

    vector<int> input;
    input.reserve(n);

    int value;
    for(int i=0; i<n; i++){
        cin >> value;
        input.push_back(value);
    }

    vector< vector<int> > res = ThreeNumSum(input);

    int res_size = res.size();
    for(int i=0; i< res_size; i++){
        const vector<int>& line = res[i];

        for(int i=0; i < 3; i++){
            cout << line[i] << " ";
        }
        cout << endl;
    }
}