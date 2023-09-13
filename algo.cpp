#include <iostream>
#include <queue>
using namespace std;

int n, m, h, ans = -1;
vector<vector<bool>> MAP;
vector<pair<int, int>> path;

void input() {
    cin >> n >> m >> h;
    MAP = vector<vector<bool>>(h, vector<bool>(--n, false));
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        MAP[a - 1][b - 1] = true;
    }
}

bool isCorrect() {
    vector<vector<bool>> tmp = MAP;
    for (auto &&i : path)
        tmp[i.first][i.second] = true;

    for (int x = 0; x < n; x++) {
        int nx = x;
        for (int y = 0; y < h; y++)
            (0 < nx && tmp[y][nx - 1]) ? nx-- : (nx < n && tmp[y][nx]) ? nx++ : nx;
        if (nx != x)
            return false;
    }
    return true;
}

void solution(int sy, int sx, int lvl, int max_lvl) {
    if (lvl == max_lvl) {
        if (isCorrect())
            ans = lvl;
        return;
    }

    for (int x = sx; x < n; x++)
        for (int y = sy; y < h; y++) {
            if (MAP[y][x] || (x > 0 && MAP[y][x - 1]) || (x < n - 1 && MAP[y][x + 1]))
                continue;

            path.push_back({y, x});
            solution(y, x, lvl + 1, max_lvl);
            path.pop_back();
            if (ans != -1)
                return;
        }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    if (isCorrect()) {
        cout << 0;
        return 0;
    }

    for (int i = 1; i <= 3; i++) {
        solution(0, 0, 0, i);
        if (ans != -1)
            break;
    }

    cout << ans;
    return 0;
}