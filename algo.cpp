#include <iostream>
#include <queue>
using namespace std;

/*
    1. 회전은 독립적 -> row 부분만 회전하면 됨
    2. 0은 시계, 1은 반시계
*/

int n, m, q, a, d, k;
vector<vector<int>> MAP;

void input() {
    cin >> n >> m >> q;
    MAP = vector<vector<int>>(n, vector<int>(m));
    for (int y = 0; y < n; y++)
        for (int x = 0; x < m; x++)
            cin >> MAP[y][x];
}

void rotate(int plate, int dir, int cnt) {
    for (int i = 0; i < cnt; i++)
        for (int j = 0; j < m - 1; j++)
            (dir) ? swap(MAP[plate][j], MAP[plate][j + 1]) : swap(MAP[plate][m - 1 - j], MAP[plate][m - 2 - j]);
}

void erase_num() {
    vector<vector<bool>> erase_MAP(n, vector<bool>(m, false));
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < m; x++) {
        }
    }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    for (int i = 0; i < q; i++) {
        cin >> a >> d >> k;
        for (int j = a - 1; j < n; j += a)
            rotate(j, d, k);
        erase_num();
    }

    int ans = 0;
    for (int y = 0; y < n; y++)
        for (int x = 0; x < m; x++)
            ans += MAP[y][x];
    cout << ans;
    return 0;
}