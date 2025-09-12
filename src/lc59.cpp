#include <boost/format/format_fwd.hpp>
#include <iostream>
#include <cassert>
#include <type_traits>
#include <vector>
#include <unordered_map>
#include <boost/format.hpp>

// leetcode 59. 螺旋矩阵 II


using namespace std;

const int EMPTY_VALUE = -1;
const bool DEBUG = false;

int n = 0;

enum class Direction{
    Right,
    Down,
    Left,
    Up
};

void PrintIfDebug(const string& str){
    if(DEBUG){
        cout << str << endl;
    }
}

pair<int, int> CalcNextXY(int x, int y, Direction direction){
    switch(direction){
        case Direction::Right:
            y += 1; 
            break;

        case Direction::Down:
            x += 1;
            break;

        case Direction::Left:
            y -= 1;
            break;

        case Direction::Up:
            x -= 1;
            break;

        default:
            assert(0); break;
    }

    return {x, y};
}

inline bool IsValid(const int x, const int y){
    return x >= 0 && x < n && y >= 0 && y < n;
}

Direction ChangeDirection(const Direction currentDirection){
     switch(currentDirection){
        case Direction::Right:
            return Direction::Down; 
            break;

        case Direction::Down:
            return Direction::Left;
            break;

        case Direction::Left:
            return Direction::Up;
            break;

        case Direction::Up:
            return Direction::Right;
            break;

        default:
            assert(0); break;
    }
}

vector<vector<int>> GetResult(){
    int x = 0;
    int y = 0;
    Direction curDir = Direction::Right; // current direction
    vector<vector<int>> result = vector<vector<int>>(n, vector<int>(n, EMPTY_VALUE));

    for(int i = 1; i <= n * n; i++){
        result[x][y] = i;

        auto [next_x, next_y] = CalcNextXY(x, y, curDir);

        PrintIfDebug((boost::format("i:%d x:%d y:%d next_x:%d next_y:%d dir:%d") % i % x % y % next_x % next_y % static_cast<int>(curDir)).str());

        if( (!IsValid(next_x, next_y)) || (result[next_x][next_y] != EMPTY_VALUE)){
            //换方向
            curDir = ChangeDirection(curDir);
            //重试
            auto p = CalcNextXY(x, y, curDir);
            next_x = p.first;
            next_y = p.second;

            if (i < n*n){
                assert(IsValid(next_x, next_y) && (result[next_x][next_y] == EMPTY_VALUE));
            }
        }

        x = next_x;
        y = next_y;
    }

    return result;
}

int main(){
    cout << "please input a postive number between 1 and 100:"; 
    cin >> n;
    assert(n > 0 && n < 100);

    auto result = GetResult();

    for(const auto& vec : result){
        for(const int elem : vec){
            cout << boost::format("%3d") % elem << " ";
        }
        cout << endl;
    }
}