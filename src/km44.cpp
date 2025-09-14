#include <iostream>
#include <cassert>

using namespace std;

/**
    题目链接: https://kamacoder.com/problempage.php?pid=1044
    其实就是简单的二维区间和
 */

const int N = 110;
const int M = 110;
const int INF = numeric_limits<int>::max();

int a[N][M];
int n, m;

int main(){
    cin >> n >> m;

    assert(n > 1 || m > 1);

    // 构造二维区间和矩阵
    for(int i=1; i<=n; i++){
        for(int j=1; j<=m; j++){
            cin >> a[i][j];
            a[i][j] = a[i][j] + a[i-1][j] + a[i][j-1] - a[i-1][j-1];
        }
    }

    int result = INF;

    // 横向分割
    for(int i = 1; i<n; i++){
        int up = a[i][m];
        int down = a[n][m] - a[i][m];
        int candidate = abs(up - down);

        result =  min(result, candidate);
    }

    // 纵向分割
    for(int j = 1; j < m; j++){
        int left  = a[n][j];
        int right = a[n][m] - a[n][j];
        int candidate = abs(left - right);

        result =  min(result, candidate);
    }   

    cout << result << endl;
}