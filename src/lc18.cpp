/**
    原题链接：https://leetcode.cn/problems/4sum/description/
*/

/**
    要点：四数之和，实际上是一个三数之和，但是有很多坑，要考虑边界条件！
*/

/**
    input格式: 先输入n, 然后输入n个整数，表示数组的值, 然后输出target

    提示： 1 <= n <= 200
          每个数都是 -10^9 ~ 10^9
*/

/**
    测试用例:
    1. 6 1,0,-1,0,-2,2 0    期待结果: [[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]
    2. 5 2,2,2,2,2   8      期待结果: [[2,2,2,2]]
    3. 1 0 0                期待结果: 空
    4. 4 1000000000,1000000000,1000000000,1000000000 -294967296 期待结果: 空
*/

#include <iostream> 
#include <vector>
#include <algorithm>

using namespace std;

#define assert(condition) do { \
    if((! (condition) )){ \
        cout << "Assert Failed: " << #condition << endl;\
        std::abort(); \
    } \
} while(0)

const bool DEBUG = false;

static inline void PrintVec(const vector<int>& vec){
    if(!DEBUG) return;

    std::for_each(vec.begin(), vec.end(), [](int x){ cout << x << " "; });
    cout << endl;
}

static inline void PrintMatrix(const vector<vector<int>>& nums){
    if(!DEBUG) return;

    if(nums.size() < 1){
        cout << "Empty" << endl;
        return;
    }

    std::for_each(nums.begin(), nums.end(), [](const vector<int>& vec) {PrintVec(vec);});
}

static inline void UpdateHead(const vector<int>& nums, int& head ,const int& tail){
    while(head < tail && nums[head] == nums[head + 1]){
        ++head;
    }
    ++head;
}

static inline void UpdateTail(const vector<int>& nums, const int& head ,int& tail){
    while(head < tail && nums[tail] == nums[tail - 1]){
        --tail;
    }
    --tail;
}

vector<vector<int>> FourSum(vector<int>& nums, int target){
    vector<vector<int>> result;

    std::sort(nums.begin(), nums.end());
    int n = nums.size();
    
    for(int i = 0; i < n-3; i++){
        int threeSumTarget = target - nums[i];
        for(int j = i+1; j < n-2; j++){
            int head = j + 1;
            int tail = n - 1;

            while(head < tail){
                int threeSum = nums[j] + nums[head] + nums[tail];
                if(threeSum == threeSumTarget){
                    // 优化成emplace_back
                    result.push_back({nums[i], nums[j], nums[head], nums[tail]});
                    UpdateHead(nums, head, tail);
                    UpdateTail(nums, head, tail);
                } else if(threeSum > threeSumTarget){
                    UpdateTail(nums, head, tail);
                } else{
                    UpdateHead(nums, head, tail);
                }
            }

            while(j < n - 2 && nums[j] == nums[j + 1]){
                ++j;
            }
        }

        while(i < n-3 && nums[i] == nums[i+1]){
            ++i;
        }
    }

    return result;
}

int main(){
    int n;
    cin >> n;
    assert(n > 0);

    vector<int> vec(n, 0);
    for(int i=0; i<n; i++){
        cin >> vec[i];
    }

    int target;
    cin >> target;

    auto result = FourSum(vec, target);

    PrintMatrix(result); 
}