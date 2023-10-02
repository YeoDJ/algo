#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

struct player {
    int y, x, dir, stat;
    int gun, score;
};

int dy[] = {-1, 0, 1, 0};
int dx[] = {0, 1, 0, -1};
int n, m, k;
vector<vector<vector<int>>> MAP;
vector<player> arr;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n; }

void input() {
    cin >> n >> m >> k;
    MAP = vector<vector<vector<int>>>(n, vector<vector<int>>(n));
    arr = vector<player>(m);

    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i]) {
            int g;
            cin >> g;
            if (g)
                j.push_back(g);
        }
    for (auto &&i : arr) {
        cin >> i.y >> i.x >> i.dir >> i.stat;
        i.y--, i.x--;
        i.score = i.gun = 0;
    }
}

int isTwoPlayer(player p, int idx) {
    for (int i = 0; i < m; i++)
        if (i != idx && arr[i].y == p.y && arr[i].x == p.x)
            return i;
    return -1;
}

void move_player(player &p, bool isFight, int idx) {
    player tmp = p;
    tmp.y = p.y + dy[p.dir];
    tmp.x = p.x + dx[p.dir];

    if (!isFight && !inRange(tmp.y, tmp.x))
        p.dir += (p.dir >= 2) ? -2 : 2;
    else if (isFight)
        for (int i = 0; i < 4; i++) {
            if (inRange(tmp.y, tmp.x) && isTwoPlayer(tmp, idx) < 0)
                break;
            p.dir = (p.dir == 3) ? 0 : p.dir + 1;
            tmp.y = p.y + dy[p.dir];
            tmp.x = p.x + dx[p.dir];
        }
    p.y += dy[p.dir], p.x += dx[p.dir];
}

void get_gun(player &p) {
    if (p.gun)
        MAP[p.y][p.x].push_back(p.gun);
    sort(MAP[p.y][p.x].begin(), MAP[p.y][p.x].end());
    p.gun = MAP[p.y][p.x][MAP[p.y][p.x].size() - 1];
    MAP[p.y][p.x].pop_back();
}

// p1이면 true, p2이면 false
bool isWinner(player p1, player p2) {
    int s1 = p1.stat + p1.gun, s2 = p2.stat + p2.gun;
    if (s1 == s2)
        return p1.stat > p2.stat;
    return s1 > s2;
}

void fight_player(player &p1, player &p2, int idx) {
    int score = abs(p1.stat + p1.gun - (p2.stat + p2.gun));
    if (isWinner(p1, p2)) {
        // 진 플레이어
        MAP[p2.y][p2.x].push_back(p2.gun);
        p2.gun = 0;
        move_player(p2, true, idx);
        if (MAP[p2.y][p2.x].size())
            get_gun(p2);

        // 이긴 플레이어
        p1.score += score;
        get_gun(p1);
    } else {
        // 진 플레이어
        MAP[p1.y][p1.x].push_back(p1.gun);
        p1.gun = 0;
        move_player(p1, true, idx);
        if (MAP[p1.y][p1.x].size())
            get_gun(p1);

        // 이긴 플레이어
        p2.score += score;
        get_gun(p2);
    }
}

void debug() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!MAP[i][j].size())
                cout << 0;
            else
                for (auto &&k : MAP[i][j])
                    cout << k;
            cout << ' ';
        }
        cout << endl;
    }

    cout << endl;
    for (auto &&i : arr)
        cout << i.y << ' ' << i.x << ' ' << i.dir << ' ' << i.stat << ' ' << i.gun << endl;
    cout << endl;
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < m; j++) {
            move_player(arr[j], false, j);
            int idx = isTwoPlayer(arr[j], j);
            if (MAP[arr[j].y][arr[j].x].size() && idx < 0)
                get_gun(arr[j]);
            else if (idx >= 0)
                fight_player(arr[j], arr[idx], idx);
            debug();
        }
    }

    for (auto &&i : arr)
        cout << i.score << ' ';
    return 0;
}