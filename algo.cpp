#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

// 0: 길, 1: 베이스캠프, 2: 편의점 도착
int dy[] = {-1, 0, 0, 1};
int dx[] = {0, -1, 1, 0};
int n, m, ans = 0;
vector<vector<int>> MAP, wall;

// 현재 손놈이 있는 곳, 손놈이 갈 목적지
vector<pair<int, int>> man, dest;
// 편의점을 찾으러 투입되었는가?
vector<bool> isMan;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n && !wall[y][x]; }

void input() {
    cin >> n >> m;
    MAP = vector<vector<int>>(n, vector<int>(n, 0));
    wall = MAP;
    dest = vector<pair<int, int>>(m, {n, n});
    man = dest;
    isMan = vector<bool>(m, false);

    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i])
            cin >> j;
    for (auto &&i : dest) {
        cin >> i.first >> i.second;
        i.first--, i.second--;
    }
}

int bfs(pair<int, int> p, int idx) {
    queue<pair<int, int>> q;
    vector<vector<int>> used(n, vector<int>(n, 0));
    vector<vector<int>> dist = used;
    q.push(p);

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int y = cur.first + dy[i];
            int x = cur.second + dx[i];
            if (inRange(y, x) && !used[y][x]) {
                used[y][x] = 1;
                dist[y][x] = dist[cur.first][cur.second] + 1;
                q.push({y, x});
            }
            if (cur == dest[idx])
                return dist[cur.first][cur.second];
        }
    }

    return n * n;
}

pair<int, int> move(int p) {
    pair<int, int> ans = man[p];
    int min_len = n * n;

    for (int i = 0; i < 4; i++) {
        int y = man[p].first + dy[i];
        int x = man[p].second + dx[i];
        if (inRange(y, x)) {
            int len = bfs({y, x}, p);
            if (len < min_len) {
                min_len = len;
                ans = {y, x};
            }
        }
    }
    return ans;
}

pair<int, int> set_person(int p) {
    pair<int, int> ans = man[p];
    int min_len = n * n;

    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            if (inRange(y, x) && MAP[y][x] == 1) {
                int len = bfs({y, x}, p);
                if (len < min_len) {
                    min_len = len;
                    ans = {y, x};
                }
            }
    return ans;
}

// 만약 모두 편의점에 들어갔다면 빠져나온다
bool isEmpty() {
    for (int i = 0; i < m; i++)
        if (isMan[i] || man[i].first == n || man[i].second == n || MAP[man[i].first][man[i].second] != 2)
            return true;
    return false;
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    while (isEmpty()) {
        // 최단거리 움직이기
        for (int i = 0; i < m; i++)
            if (isMan[i])
                man[i] = move(i);

        // 도착했는가?
        for (int i = 0; i < m; i++)
            if (isMan[i] && man[i] == dest[i]) {
                wall[man[i].first][man[i].second] = 1;
                MAP[man[i].first][man[i].second] = 2;
                isMan[i] = false;
            }

        // 투입할 베이스캠프 위치
        if (ans < m && !isMan[ans]) {
            isMan[ans] = true;
            man[ans] = set_person(ans);
            wall[man[ans].first][man[ans].second] = 1;
        }
        ans++;
    }

    cout << ans;
    return 0;
}