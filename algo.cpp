#include <iostream>
#include <queue>
using namespace std;

int n, m, k;
vector<vector<int>> MAP, player;

void input() {
    cin >> n >> m >> k;
    MAP = vector<vector<int>>(n, vector<int>(n, 0));
    player = MAP;

    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i])
            cin >> j;
    for (int i = 0; i < m; i++) {
        int y, x;
        cin >> y >> x;
        player[--y][--x] = i + 1;
    }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    for (int i = 0; i < k; i++) {
        /* code */
    }

    int ans;
    pair<int, int> ans_p;
    cout << ans << endl << ans_p.first << ' ' << ans_p.second;
    return 0;
}