#include <iostream>
#include <queue>
using namespace std;

int n, lvl = 2, ans = 0;
vector<vector<int>> MAP, used;
vector<vector<pair<int, int>>> path;

void input() {
    cin >> n;
    MAP = vector<vector<int>>(n, vector<int>(n));
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            cin >> MAP[y][x];
}

bool bfs() {
    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};
    queue<pair<int, int>> q;

    while (!q.empty()) {
        /* code */
    }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    while (1) {
        // 1. used ¼³Á¤
        used = vector<vector<int>>(n, vector<int>(n, 0));
        for (int y = 0; y < n; y++)
            for (int x = 0; x < n; x++)
                if (MAP[y][x] > lvl)
                    used[y][x] = 1;
        if (!bfs())
            break;
        ans++;
    }

    cout << ans;
    return 0;
}