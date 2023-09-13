#include <iostream>
#include <queue>
using namespace std;

int n, m, k;
vector<vector<int>> food;
vector<vector<vector<int>>> virus;

void input() {
    cin >> n >> m >> k;
    food = vector<vector<int>>(n, vector<int>(n));
    virus = vector<vector<vector<int>>>(n, vector<vector<int>>(n));

    for (int i = 0; i < n; i++)
        for (auto &&j : food[i])
            cin >> j;
    for (int i = 0; i < m; i++) {
        int y, x, v;
        cin >> y >> x >> v;
        virus[y - 1][x - 1].push_back(v);
    }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    for (int i = 0; i < k; i++) {
        /* code */
    }

    return 0;
}