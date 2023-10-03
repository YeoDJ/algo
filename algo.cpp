#include <iostream>
#include <queue>
using namespace std;

int n, m;
int dy[] = {-1, 0, 0, 1};
int dx[] = {0, -1, 1, 0};
vector<vector<int>> MAP;
vector<vector<vector<int>>> customer;

void input() {
    cin >> n >> m;
    MAP = vector<vector<int>>(n, vector<int>(n));
    customer = vector<vector<vector<int>>>(n, vector<vector<int>>(n));
    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i])
            cin >> j;
    for (int i = 0; i < m; i++) {
        int y, x;
        cin >> y >> x;
        customer[y][x].push_back(i + 1);
    }
}

int main() {
    freopen("./input.txt", "r", stdin);
    return 0;
}