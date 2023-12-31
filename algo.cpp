#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
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
map<int, pair<int, int>> santa; // 산타 위치
set<int> set_santa;             // 산타 목록
unordered_map<int, int> stun;   // 산타 스턴 여부(스턴 시간 2초)
map<int, int> score;            // 산타 점수

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
        set_santa.insert(pn);
    }
    update();
}

void interaction(pair<int, int> cur_p, int dir) {
    while (1) {
        int pn = MAP[cur_p.first][cur_p.second];
        cur_p.first += dy[dir], cur_p.second += dx[dir];
        santa[pn] = cur_p;
        if (!inRange(cur_p.first, cur_p.second) || !MAP[cur_p.first][cur_p.second]) {
            if (!inRange(cur_p.first, cur_p.second))
                santa.erase(pn), stun.erase(pn);
            return;
        }
    }
}

void hit(pair<int, int> next_p, int pn, int dir, int power) {
    deer = next_p;
    score[pn] += power;
    int y = next_p.first + dy[dir] * power;
    int x = next_p.second + dx[dir] * power;

    // 산타가 격자 밖으로 나가면 지도에서 제외됨
    if (!inRange(y, x)) {
        santa.erase(pn), stun.erase(pn);
        return;
    }

    // 산타를 옮긴 뒤 스턴을 건다.
    santa[pn] = {y, x}, stun[pn] = 2;
    if (MAP[y][x] > 0 && MAP[y][x] != pn)
        interaction({y, x}, dir);
}

void move_deer() {
    int y, x, dir;
    int min_len = INT32_MAX, len, target = -1;
    pair<int, int> next_deer = deer;

    // 루돌프와 산타가 가장 가까운 경우(우선순위 따지며)
    for (auto &&i : santa) {
        len = find_len(deer, i.second);
        if (len <= min_len) {
            min_len = len;
            if (target == -1 || compare(i.second, santa[target]))
                target = i.first;
        }
    }
    DONT_MIN;

    // 그 산타에 가장 가까이 가기
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

    // 산타와 충돌
    if (next_deer == santa[target])
        hit(next_deer, target, dir, c);
    else
        deer = next_deer;
    update();
}

void move_santa(int pn) {
    // min_len에 지금 당장의 거리를 구한다
    // 구하는 이유: 가까워지는지 확인하기 위해
    int y, x, dir;
    int min_len = find_len(santa[pn], deer), len;
    pair<int, int> next_santa = santa[pn];

    // 루돌프와 가장 가까운 곳으로 산타 보내기
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
    if (min_len == find_len(santa[pn], deer))
        return;

    // 루돌프와 충돌
    if (next_santa == deer)
        hit(next_santa, pn, (dir <= 3) ? dir + 4 : dir - 4, d);
    else
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
        for (auto &&i : set_santa)
            if (santa.find(i) != santa.end() && !stun[i])
                move_santa(i);

        debug();
        // stun 업데이트
        for (auto &&i : stun)
            if (i.second)
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