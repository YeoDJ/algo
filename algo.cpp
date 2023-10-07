#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

struct player {
    int idx, y, x, dir;
};

int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, -1, 1};
int n, m, k;
// (독점자, 유효 시간)
vector<vector<pair<int, int>>> MAP;
vector<player> arr;
vector<vector<vector<int>>> prior_dir;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n; }

bool compare(player p1, player p2) { return p1.idx < p2.idx; }

void input() {
    cin >> n >> m >> k;
    MAP = vector<vector<pair<int, int>>>(n, vector<pair<int, int>>(n, {0, 0}));
    prior_dir = vector<vector<vector<int>>>(m, vector<vector<int>>(4, vector<int>(4)));

    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++) {
            cin >> MAP[y][x].first;
            if (MAP[y][x].first) {
                MAP[y][x].second = k;
                arr.push_back({MAP[y][x].first, y, x, -1});
            }
        }

    sort(arr.begin(), arr.end(), compare);
    for (auto &&i : arr) {
        cin >> i.dir;
        i.dir--;
    }

    for (int i = 0; i < m; i++)
        for (int j = 0; j < 4; j++)
            for (auto &&z : prior_dir[i][j]) {
                cin >> z;
                z--;
            }
}

void move_player(int i, player &p) {
    int my_land = -1;
    for (auto &&j : prior_dir[i][p.dir]) {
        int y = p.y + dy[j];
        int x = p.x + dx[j];
        if (inRange(y, x)) {
            // 미독점 구역이면 이동하고 return
            if (!MAP[y][x].first) {
                p.y = y, p.x = x;
                p.dir = j;
                return;
            }
            if (MAP[y][x].first == p.idx && my_land == -1)
                my_land = j;
        }
    }

    // 미독점 구역이 없으면 독점했던 곳으로 감
    if (my_land != -1) {
        p.y += dy[my_land], p.x += dx[my_land];
        p.dir = my_land;
    }
}

void remain_land() {
    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i])
            if (j.second > 0) {
                j.second--;
                if (!j.second)
                    j.first = 0;
            }

    // 이긴 플레이어에 대해 MAP에 반영하기(독점하기)
    for (auto &&i : arr)
        MAP[i.y][i.x] = {i.idx, k};
}

vector<player> fight_player() {
    // 싸움에서 지면 true(없어짐)
    int sz = arr.size();
    vector<bool> isFight(sz, false);
    vector<player> ans;

    // 지는 대상 선정
    for (int i = 0; i < sz - 1; i++)
        for (int j = i + 1; j < sz; j++)
            if (arr[i].y == arr[j].y && arr[i].x == arr[j].x)
                isFight[j] = true;

    // 이긴 player만 살아남음
    for (int i = 0; i < sz; i++)
        if (!isFight[i])
            ans.push_back(arr[i]);
    return ans;
}

int main() {
    input();
    int ans = 0;

    while (arr.size() != 1 && ans < 1000) {
        for (auto &&i : arr)
            move_player(i.idx - 1, i);
        arr = fight_player();
        remain_land();
        ans++;
    }

    ans = (ans == 1000) ? -1 : ans;
    cout << ans;
    return 0;
}