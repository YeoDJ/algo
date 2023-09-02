#include <iostream>
#include <queue>
using namespace std;

struct Robot {
    pair<int, int> p;
    int lvl, exp;
};

int n, ans = 0;
Robot robot;
vector<vector<int>> MAP, dist;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n; }

void input() {
    cin >> n;
    robot.exp = 0, robot.lvl = 2;
    MAP = vector<vector<int>>(n, vector<int>(n));

    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++) {
            cin >> MAP[y][x];
            if (MAP[y][x] == 9) {
                robot.p = {y, x};
                MAP[y][x] = -1;
            }
        }
}

int bfs() {
    // 1. used 설정
    vector<vector<int>> used(n, vector<int>(n, 0));
    dist = used;
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            if (MAP[y][x] > robot.lvl)
                used[y][x] = 1;

    // 2. bfs 진행하여 몬스터와의 최단거리 구하기
    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};
    queue<pair<int, int>> q;
    q.push(robot.p);

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int y = cur.first + dy[i];
            int x = cur.second + dx[i];
            if (inRange(y, x) && MAP[y][x] <= robot.lvl && !used[y][x]) {
                used[y][x] = 1;
                dist[y][x] = dist[cur.first][cur.second] + 1;
                q.push({y, x});
            }
        }
    }

    // 3. 몬스터 중 자신보다 레벨이 낮으면서 갈 수 있는 최단거리는?
    int min_dist = INT32_MAX;
    pair<int, int> p(-1, -1);
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            if (1 <= MAP[y][x] && MAP[y][x] < robot.lvl && used[y][x] && dist[y][x] < min_dist) {
                min_dist = dist[y][x];
                p = {y, x};
            }

    if (min_dist != INT32_MAX) {
        MAP[robot.p.first][robot.p.second] = 0;
        MAP[p.first][p.second] = -1;
        robot.p = p;
        robot.exp = (robot.exp == robot.lvl) ? 0 : robot.exp + 1;
        if (robot.exp == robot.lvl) {
            robot.lvl++;
            robot.exp = 0;
        }
    }
    return min_dist;
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    while (1) {
        int num = bfs();
        if (num == INT32_MAX)
            break;
        ans += num;
    }

    cout << ans;
    return 0;
}