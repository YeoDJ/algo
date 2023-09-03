#include <iostream>
#include <queue>
using namespace std;

int n, m, q, x, d, k;
vector<vector<int>> MAP;

void input() {
    cin >> n >> m >> q;
    MAP = vector<vector<int>>(n, vector<int>(4));
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            cin >> MAP[y][x];
    cin >> x >> d >> k;
}

int main() {
    input();
    return 0;
}