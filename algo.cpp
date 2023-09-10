#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

/*
    1. 경계와 안의 지역
    2. 좌상단 윗부분, 위꼭, 왼꼭
    3. 우상단 윗부분, 오른꼭, 위꼭
    4. 좌하단 아랫부분, 왼꼭, 아래꼭
    5. 우하단 아랫부분, 아래꼭, 오른꼭
*/

int dy[] = {-1, -1, 1, 1};
int dx[] = {1, -1, -1, 1};
int n, ans = INT32_MAX;
vector<int> path;
vector<vector<int>> MAP;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n; }

void input() {
    cin >> n;
    MAP = vector<vector<int>>(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i])
            cin >> j;
}

int solution(int ny, int nx) {
    int idx = 0, y, x;
    pair<int, int> ggok[4];
    vector<vector<int>> group(n, vector<int>(n, 0));
    int g_cnt[5] = {0};

    // 1. 경계선
    for (auto &&i : path) {
        for (int j = 0; j < i; j++) {
            ny += dy[idx], nx += dx[idx];
            group[ny][nx] = 1;
        }
        ggok[idx] = {ny, nx};
        idx++;
    }

    // 2. 그 안에 있는 지역
    for (y = 0; y < n; y++)
        for (x = 0; x < n; x++)
            if (group[y][x]) {
                idx = distance(group[y].begin(), find(group[y].begin() + x + 1, group[y].end(), 1));
                while (x++ != idx && idx != n)
                    group[y][x] = 1;
            }

    // 3. 나머지 땅 정하기
    for (int i = 0; i < 4; i++) {
        switch (i) {
        case 0:
            for (y = ggok[i].first; y >= 0; y--)
                for (x = (path[0] >= path[1]) ? ggok[i].second : ggok[3].second; x < n; x++)
                    if (!group[y][x])
                        group[y][x] = i + 2;
            break;
        case 1:
            for (y = (path[0] <= path[1]) ? ggok[i].first : ggok[i - 1].first; y >= 0; y--)
                for (x = ggok[i].second; x >= 0; x--)
                    if (!group[y][x])
                        group[y][x] = i + 2;
            break;
        case 2:
            for (y = ggok[i].first; y < n; y++)
                for (x = (path[0] >= path[1]) ? ggok[i].second : ggok[i - 1].second; x >= 0; x--)
                    if (!group[y][x])
                        group[y][x] = i + 2;
            break;
        case 3:
            for (y = (path[0] <= path[1]) ? ggok[i].first : ggok[i - 1].first; y < n; y++)
                for (x = ggok[i].second; x < n; x++)
                    if (!group[y][x])
                        group[y][x] = i + 2;
            break;
        }
    }

    // debug
    for (int i = 0; i < n; i++) {
        for (auto &&j : group[i]) {
            cout << j << ' ';
        }
        cout << endl;
    }
    cout << endl;

    // 4. 부족 별 인원 수 구하기
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            g_cnt[group[y][x] - 1] += MAP[y][x];

    // 5. 인원수 최대, 최소 구하여 그 차이 반환하기
    return *max_element(g_cnt, g_cnt + 5) - *min_element(g_cnt, g_cnt + 5);
}

void make_path(int lvl) {
    if (lvl == 4) {
        if (path[0] == path[2] && path[1] == path[3]) {
            pair<int, int> ry = {path[0] + path[1], n - 1};
            pair<int, int> rx = {path[3], n - 1 - path[0]};
            for (int y = ry.first; y <= ry.second; y++)
                for (int x = rx.first; x <= rx.second; x++)
                    ans = min(ans, solution(y, x));
        }
        return;
    }

    for (int i = 1; i <= n / 2; i++) {
        path.push_back(i);
        make_path(lvl + 1);
        path.pop_back();
    }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();
    make_path(0);
    cout << ans;
    return 0;
}