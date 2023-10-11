#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

/*
    <오답 노트>
    1. 가장 핵심인 변수: prevPos
        ===> 방향 우선순위에 맞추어 현재 위치 '이전의' 위치를 따로 저장하여 경로 파악
        ===> 경로를 구할 때 이 변수를 도착점 -> 시작점 역순으로 탐색
    2. dist 초기 값을 -1로 함
        ===> 아예 지나가지 않은 경우는 -1이며, 시작점은 0으로 setting
        ===> 도착점의 dist 값이 -1이면 레이저가 도달하지 못했다는 뜻
*/

int dy[] = {0, 1, 0, -1, -1, -1, 1, 1};
int dx[] = {1, 0, -1, 0, -1, 1, 1, -1};
int n, m, k;
// attack_his: 숫자가 클 수록 공격한 지 오래됨
// path: 공격에 관련된 지역 좌표들
vector<vector<int>> MAP, attack_his;
vector<pair<int, int>> path;

bool compare(pair<int, int> p1, pair<int, int> p2) {
    if (MAP[p1.first][p1.second] == MAP[p2.first][p2.second]) {
        if (attack_his[p1.first][p1.second] == attack_his[p2.first][p2.second]) {
            if (p1.first + p1.second == p2.first + p2.second)
                return p1.second > p2.second;
            return p1.first + p1.second > p2.first + p2.second;
        }
        return attack_his[p1.first][p1.second] < attack_his[p2.first][p2.second];
    }
    return MAP[p1.first][p1.second] < MAP[p2.first][p2.second];
}

bool isRemainOne() {
    int cnt = 0;
    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i]) {
            if (j)
                cnt++;
            if (cnt > 1)
                return false;
        }
    return true;
}

void input() {
    cin >> n >> m >> k;
    MAP = vector<vector<int>>(n, vector<int>(m, 0));
    attack_his = MAP;
    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i])
            cin >> j;
}

pair<int, int> find_attacker() {
    pair<int, int> ans(-1, -1);
    for (int y = 0; y < n; y++)
        for (int x = 0; x < m; x++) {
            if (!MAP[y][x])
                continue;
            if (ans == make_pair(-1, -1) || compare({y, x}, ans))
                ans = {y, x};
        }

    MAP[ans.first][ans.second] += n + m;
    return ans;
}

bool laserAttack(pair<int, int> p, pair<int, int> target) {
    vector<vector<pair<int, int>>> prevPos(n, vector<pair<int, int>>(m, {-1, -1}));
    vector<vector<int>> dist(n, vector<int>(m, -1));
    queue<pair<int, int>> q;
    pair<int, int> cur;

    // 초기 세팅
    q.push(p), dist[p.first][p.second] = 0;

    // 최단경로 찾기
    while (!q.empty()) {
        cur = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int y = cur.first + dy[i];
            int x = cur.second + dx[i];
            y = (y >= n) ? 0 : (y < 0) ? n - 1 : y;
            x = (x >= m) ? 0 : (x < 0) ? m - 1 : x;

            if (MAP[y][x] && dist[y][x] == -1) {
                dist[y][x] = dist[cur.first][cur.second] + 1;
                prevPos[y][x] = cur;
                q.push({y, x});
            }
        }
    }
    // 레이저가 도달하지 못했으면 return
    if (dist[target.first][target.second] < 0)
        return false;

    // 일단 타겟 맞춘 다음
    MAP[target.first][target.second] -= MAP[p.first][p.second];
    if (MAP[target.first][target.second] <= 0)
        MAP[target.first][target.second] = 0;

    // 경로를 구한다(단, 시작점은 뺀다.)
    cur = target;
    while (cur != p) {
        path.push_back(prevPos[cur.first][cur.second]);
        cur = prevPos[cur.first][cur.second];
    }
    path.pop_back();

    // 경로를 따라 공격자의 절반만큼 공격한다.
    for (auto &&i : path) {
        MAP[i.first][i.second] -= MAP[p.first][p.second] / 2;
        if (MAP[i.first][i.second] <= 0)
            MAP[i.first][i.second] = 0;
    }
    path.push_back(p), path.push_back(target);
    return true;
}

void bomb(pair<int, int> p, pair<int, int> target) {
    for (int i = 0; i < 8; i++) {
        int y = target.first + dy[i];
        int x = target.second + dx[i];
        y = (y >= n) ? 0 : (y < 0) ? n - 1 : y;
        x = (x >= m) ? 0 : (x < 0) ? m - 1 : x;
        if (MAP[y][x] && make_pair(y, x) != p) {
            path.push_back({y, x});
            MAP[y][x] -= MAP[p.first][p.second] / 2;
            if (MAP[y][x] <= 0)
                MAP[y][x] = 0;
        }
    }

    MAP[target.first][target.second] -= MAP[p.first][p.second];
    if (MAP[target.first][target.second] <= 0)
        MAP[target.first][target.second] = 0;
    path.push_back(p), path.push_back(target);
}

void attack(pair<int, int> p) {
    // 1. 공격할 포탑 선정
    pair<int, int> ans(-1, -1);
    for (int y = 0; y < n; y++)
        for (int x = 0; x < m; x++) {
            if (!MAP[y][x] || make_pair(y, x) == p)
                continue;
            if (ans == make_pair(-1, -1) || !compare({y, x}, ans))
                ans = {y, x};
        }
    // 2. 레이저 공격 -> 실패하면 폭탄 공격
    if (!laserAttack(p, ans))
        bomb(p, ans);
    attack_his[p.first][p.second] = -1;
}

void reload() {
    // 1. 아무 연관 없는 구역은 공격력 1 증가
    for (int y = 0; y < n; y++)
        for (int x = 0; x < m; x++)
            if (MAP[y][x] && find(path.begin(), path.end(), make_pair(y, x)) == path.end())
                MAP[y][x]++;
    // 2. 공격 이력 갱신하기
    for (int i = 0; i < n; i++)
        for (auto &&j : attack_his[i])
            j++;
}

int output() {
    int max_cnt = -1;
    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i])
            max_cnt = max(max_cnt, j);
    return max_cnt;
}

int main() {
    input();
    for (int i = 0; i < k; i++) {
        path.clear();
        attack(find_attacker());
        reload();
        if (isRemainOne())
            break;
    }

    cout << output();
    return 0;
}