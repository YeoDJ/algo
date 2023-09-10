#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// d: 우좌상하
// 0: 위에 쌓는다, 1: 빨간 칸에 가기 전에 순서 반전, 2: 벽(격자 바운더리와 같은 개념)
int dy[] = {0, 0, -1, 1};
int dx[] = {1, -1, 0, 0};
int n, k;
vector<vector<int>> color_MAP;
vector<vector<vector<pair<int, int>>>> mal_MAP;

void input() {
    cin >> n >> k;
    color_MAP = vector<vector<int>>(n, vector<int>(n));
    mal_MAP = vector<vector<vector<pair<int, int>>>>(n, vector<vector<pair<int, int>>>(n));
    for (int i = 0; i < n; i++)
        for (auto &&j : color_MAP[i])
            cin >> j;

    for (int i = 0; i < k; i++) {
        int y, x, d;
        cin >> y >> x >> d;
        mal_MAP[y - 1][x - 1].push_back({i, d - 1});
    }
}

bool isWall(int y, int x) {
    if (y < 0 || y >= n || x < 0 || x >= n)
        return true;
    return color_MAP[y][x] == 2;
}

bool isOverlap() {
    for (int i = 0; i < n; i++)
        for (auto &&j : mal_MAP[i])
            if (j.size() >= 4)
                return true;
    return false;
}

void mal_move(int y, int x, int i) {
    int ny = y + dy[mal_MAP[y][x][i].second];
    int nx = x + dx[mal_MAP[y][x][i].second];
    vector<pair<int, int>> tmp;
    if (isWall(ny, nx))
        (mal_MAP[y][x][i].second % 2) ? mal_MAP[y][x][i].second-- : mal_MAP[y][x][i].second++;

    ny = y + dy[mal_MAP[y][x][i].second];
    nx = x + dx[mal_MAP[y][x][i].second];
    if (isWall(ny, nx))
        return;

    for (int j = i; j >= 0; j--) {
        tmp.push_back(mal_MAP[y][x][j]);
        mal_MAP[y][x].erase(mal_MAP[y][x].begin() + j);
    }

    int sz = tmp.size();
    for (int j = 0; j < sz; j++)
        mal_MAP[ny][nx].insert(mal_MAP[ny][nx].begin(), (!color_MAP[ny][nx]) ? tmp[j] : tmp[sz - 1 - j]);
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    int ans = 0;
    while (!isOverlap() && ans <= 1000) {
        ans++;
        for (int i = 0; i < k; i++) {
            bool flag = false;
            for (int y = 0; y < n; y++) {
                for (int x = 0; x < n; x++) {
                    auto it = find_if(mal_MAP[y][x].begin(), mal_MAP[y][x].end(), [i](const pair<int, int> &element) { return element.first == i; });
                    if (flag = it != mal_MAP[y][x].end()) {
                        mal_move(y, x, distance(mal_MAP[y][x].begin(), it));
                        break;
                    }
                }
                if (flag)
                    break;
            }
            if (isOverlap())
                break;
        }
    }

    ans = (ans >= 1000) ? -1 : ans;
    cout << ans;
    return 0;
}