#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <unordered_map>
#define DONT_MIN                                                                                                                                                                                       \
    if (min_len == INT32_MAX)                                                                                                                                                                          \
    return
using namespace std;

int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
int n, m, p, c, d;
pair<int, int> deer;            // 루돌프 위치
vector<vector<int>> MAP;        // 지도
map<int, pair<int, int>> santa; // 산타의 위치
unordered_map<int, int> stun;   // 산타 스턴 시간(기본 2초)
unordered_map<int, int> score;  // 산타 점수

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n; }

int find_len(pair<int, int> p1, pair<int, int> p2) { return pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2); }

bool compare(pair<int, int> p1, pair<int, int> p2) {
    int len1 = find_len(deer, p1);
    int len2 = find_len(deer, p2);
    if (len1 != len2)
        return len1 < len2;
    if (p1.first != p2.first)
        return p1.first > p2.first;
    return p1.second > p2.second;
}

void update() {
    MAP = vector<vector<int>>(n, vector<int>(n, 0));
    MAP[deer.first][deer.second] = -1;
    for (auto &&i : santa)
        MAP[i.second.first][i.second.second] = i.first;
}

void input() {
    cin >> n >> m >> p >> c >> d;
    cin >> deer.first >> deer.second;
    --deer.first, --deer.second;
    for (int i = 0; i < p; i++) {
        int pn, y, x;
        cin >> pn >> y >> x;
        santa[pn] = {--y, --x};
        stun[pn] = score[pn] = 0;
    }
    update();
}

void interaction(pair<int, int> cur_p, int pn, int dir) {

}

bool hit(pair<int, int> next_p, int pn, int &dir, int power) {
    deer = next_p;
    score[pn] += power;
    dir += (dir <= 3) ? 4 : -4;
    int y = next_p.first + dy[dir] * power;
    int x = next_p.second + dx[dir] * power;

    // 산타가 격자 밖으로 나간다면 MAP에서 제외된다.
    if (!inRange(y, x)) {
        santa.erase(pn), stun.erase(pn);
        return false;
    }

    // 충돌한 산타는 이동하면서 스턴이 걸린다.
    santa[pn] = {y, x}, stun[pn] = 2;
    return MAP[y][x];
}

void move_deer() {
    int y, x, dir;
    int min_len = INT32_MAX, len, target = -1;
    pair<int, int> next_deer = deer;

    // 산타와 가장 가까운 거리 구하기
    for (auto &&i : santa) {
        len = find_len(deer, i.second);
        if (len <= min_len) {
            min_len = len;
            if (target == -1 || compare(i.second, santa[target]))
                target = i.first;
        }
    }
    DONT_MIN;

    // 루돌프 이동
    min_len = INT32_MAX;
    for (int i = 0; i < 8; i++) {
        y = deer.first + dy[i], x = deer.second + dx[i];
        len = find_len({y, x}, santa[target]);
        if (inRange(y, x) && len < min_len) {
            min_len = len;
            next_deer = {y, x};
            dir = i;
        }
    }
    DONT_MIN;

    // 충돌하는가?
    if (next_deer == santa[target] && hit(next_deer, target, dir, c))
        interaction(next_deer, target, dir);
    else if (next_deer != deer && next_deer != deer)
        deer = next_deer;
    update();
}

void move_santa(int pn) {
    int y, x, dir;
    int min_len = INT32_MAX, len;
    pair<int, int> next_santa = santa[pn];

    // 산타 이동하기
    for (int i = 0; i < 8; i += 2) {
        y = santa[pn].first + dy[i];
        x = santa[pn].second + dx[i];
        len = find_len({y, x}, deer);
        if (inRange(y, x) && MAP[y][x] <= 0 && len < min_len) {
            min_len = len;
            next_santa = {y, x};
            dir = i;
        }
    }
    DONT_MIN;

    // 충돌하는가?
    if (next_santa == deer && hit(next_santa, pn, dir, d))
        interaction(next_santa, pn, dir);
    else if (next_santa != deer && next_santa != santa[pn])
        santa[pn] = next_santa;
    update();
}

void debug() {
    for (int i = 0; i < n; i++) {
        for (auto &&j : MAP[i])
            cout << j << ' ';
        cout << endl;
    }
    cout << endl;
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    while (m-- && !santa.empty()) {
        // 루돌프와 산타 움직이기
        move_deer();
        for (auto &&i : santa)
            if (!stun[distance(santa.begin(), find(santa.begin(), santa.end(), i))])
                move_santa(i.first);
        debug();
        // stun 지속시간 업데이트
        for (auto &&i : stun)
            i.second--;
        // 점수 업데이트
        for (auto &&i : score)
            if (santa.find(i.first) != santa.end())
                i.second++;
    }

    for (auto &&i : score)
        cout << i.second << ' ';
    return 0;
}