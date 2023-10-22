#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

// 빈 칸, 머리, 몸통, 꼬리, 이동 선
int dy[] = {0, -1, 0, 1};
int dx[] = {1, 0, -1, 0};
int n, m, k, ans = 0, Round = -1;
vector<vector<int>> MAP;
vector<pair<int, int>> head;
vector<vector<pair<int, int>>> arr;
// 꽉 차 있는 그룹인가?
vector<bool> isFull;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n; }

void update_point() {
    for (int i = 0; i < head.size(); i++) {
        int y, x, j;
        pair<int, int> cur = head[i], tmp;
        arr[i].clear();
        arr[i].push_back(cur);

        while (MAP[cur.first][cur.second] != 3) {
            for (j = 0; j < 4; j++) {
                y = cur.first + dy[j];
                x = cur.second + dx[j];
                if (inRange(y, x) && (MAP[y][x] == 2 || MAP[y][x] == 3) && find(arr[i].begin(), arr[i].end(), make_pair(y, x)) == arr[i].end()) {
                    tmp = {y, x};
                    // 몸통이면 바로 Snake 요소에 반영(최우선순위)
                    if (MAP[y][x] == 2) {
                        arr[i].push_back(tmp);
                        break;
                    }
                }
            }
            // 꼬리라면 Snake 요소에 반영
            if (j == 4)
                arr[i].push_back(tmp);
            cur = tmp;
        }
    }
}

void input() {
    cin >> n >> m >> k;
    MAP = vector<vector<int>>(n, vector<int>(n));

    // MAP 입력 및 HEAD 선언
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++) {
            cin >> MAP[y][x];
            if (MAP[y][x] == 1)
                head.push_back({y, x});
        }

    // SNAKE 정보 얻기
    int sz = head.size();
    arr = vector<vector<pair<int, int>>>(sz);
    isFull = vector<bool>(sz);
    update_point();

    for (int i = 0; i < sz; i++) {
        int j = arr[i].size() - 1;
        pair<int, int> tmp = {abs(arr[i][0].first - arr[i][j].first), abs(arr[i][0].second - arr[i][j].second)};
        isFull[i] = tmp == make_pair(1, 0) || tmp == make_pair(0, 1);
    }
}

pair<int, int> find_next(pair<int, int> h) {
    // MAP 밖이거나 그룹원이 없는 길일 때
    if (!inRange(h.first, h.second) || MAP[h.first][h.second] == 4)
        return make_pair(-1, -1);
    for (int i = 0; i < 4; i++) {
        int y = h.first + dy[i];
        int x = h.second + dx[i];
        if (inRange(y, x) && (MAP[y][x] == 2 || MAP[y][x] == 3))
            return make_pair(y, x);
    }
    return make_pair(-1, -1);
}

void move() {
    for (int i = 0; i < head.size(); i++) {
        int y, x, j;
        pair<int, int> prev_p = {-1, -1}, cur_p = head[i], next_p = find_next(head[i]), tmp;
        // 꽉 찬 그룹일 때
        if (isFull[i]) {
            prev_p = arr[i][0];
            cur_p = arr[i][arr[i].size() - 1];
            for (j = 0; j < 4; j++) {
                y = cur_p.first + dy[j];
                x = cur_p.second + dx[j];
                // 꼬리 이전의 몸통을 찾은 뒤
                if (inRange(y, x) && MAP[y][x] == 2) {
                    next_p = {y, x};
                    break;
                }
            }
            // MAP에 반영한다.
            swap(MAP[cur_p.first][cur_p.second], MAP[next_p.first][next_p.second]);
            swap(MAP[prev_p.first][prev_p.second], MAP[cur_p.first][cur_p.second]);
            head[i] = cur_p;
            continue;
        }

        // prev_p 초기 세팅
        for (j = 0; j < 4; j++) {
            y = cur_p.first + dy[j];
            x = cur_p.second + dx[j];
            if (inRange(y, x) && (MAP[y][x] == 2 || MAP[y][x] == 4)) {
                tmp = {y, x};
                if (MAP[y][x] == 4) {
                    head[i] = {y, x};
                    swap(MAP[y][x], MAP[cur_p.first][cur_p.second]);
                    prev_p = cur_p, cur_p = next_p;
                    next_p = find_next(cur_p);
                    break;
                }
            }
        }

        // 좌표 반영
        while (1) {
            swap(MAP[prev_p.first][prev_p.second], MAP[cur_p.first][cur_p.second]);
            if (next_p == make_pair(-1, -1))
                break;
            prev_p = cur_p, cur_p = next_p;
            next_p = find_next(cur_p);
        }
    }
}

bool throw_ball(int dir, int line) {
    // 4의 배수 별로 Round를 어떻게 진행할 것인지 설정한다.
    int y = (dir == 1) ? n - 1 : (dir == 2) ? n - 1 - line : (dir == 3) ? 0 : line;
    int x = (dir == 1) ? line : (dir == 2) ? n - 1 : (dir == 3) ? n - 1 - line : 0;
    for (int i = 0; i < n; i++) {
        if (1 <= MAP[y][x] && MAP[y][x] <= 3) {
            int idx = 0;
            for (auto &&j : arr) {
                auto it = find(j.begin(), j.end(), make_pair(y, x));
                if (it != j.end()) {
                    // 그룹원이 닿이면 점수를 얻고,
                    ans += (distance(j.begin(), it) + 1) * (distance(j.begin(), it) + 1);
                    // 그룹의 순서를 뒤집는다.
                    reverse(j.begin(), j.end());
                    head[idx] = j[0];
                    MAP[j[0].first][j[0].second] = 1;
                    MAP[j[j.size() - 1].first][j[j.size() - 1].second] = 3;
                    return true;
                }
                idx++;
            }
        }
        y += dy[dir], x += dx[dir];
    }
    return false;
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    for (int i = 0; i < k; i++) {
        move();
        update_point();
        Round = (Round == 4 * n - 1) ? 0 : Round + 1;
        throw_ball(Round / n, Round % n);
    }

    cout << ans;
    return 0;
}