#include <algorithm>
#include <iostream>
#include <queue>
#include <tuple>
using namespace std;

// 도착점을 -1로 한다.
int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, -1, 1};
int n, m, k, ans = 0;
pair<int, int> exit_p;
vector<vector<int>> MAP;
vector<pair<int, int>> player;

bool inRange(int y, int x) { return (0 <= y && y < n && 0 <= x && x < n) && !MAP[y][x]; }

void input() {
    cin >> n >> m >> k;
    MAP = vector<vector<int>>(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i])
            cin >> j;
    for (int i = 0; i < m; i++) {
        int y, x;
        cin >> y >> x;
        player.push_back({--y, --x});
    }

    cin >> exit_p.first >> exit_p.second;
    MAP[--exit_p.first][--exit_p.second] = -1;
}

void move_player() {
    int i = 0;
    while (i < player.size()) {
        int min_len = INT32_MAX;
        pair<int, int> min_p = player[i];
        for (int dir = 0; dir < 4; dir++) {
            int ny = player[i].first + dy[dir];
            int nx = player[i].second + dx[dir];
            int len = abs(exit_p.first - ny) + abs(exit_p.second - nx);
            if (len < min_len) {
                min_len = len;
                min_p = {ny, nx};
            }
        }
        if (inRange(min_p.first, min_p.second)) {
            ans++;
            if (min_p == exit_p)
                player.erase(player.begin() + i--);
            else
                player[i] = min_p;
        }
        i++;
    }
}

void rotate_MAP() {
    sort(player.begin(), player.end());
    int min_len = n, y, x;
    vector<vector<int>> tmp_MAP = MAP;
    vector<pair<int, int>> tmp_player = player;
    pair<int, int> p = player[0];

    // 출구를 기준으로 영역의 크기를 구한다.
    for (auto &&i : player) {
        int len = max(abs(exit_p.first - i.first), abs(exit_p.second - i.second)) + 1;
        if (len < min_len) {
            min_len = len;
            p = i;
        }
    }

    // 좌상단 좌표를 구한다.(y, x 순서대로)
    p.first = (p.first - min_len < 0) ? 0 : abs(p.first - exit_p.first);
    p.second = (p.second - min_len < 0) ? 0 : abs(p.second - exit_p.second);

    // 회전
    for (y = p.first; y < p.first + min_len; y++)
        for (x = p.second; x < p.second + min_len; x++) {
            pair<int, int> nyam = {y, x};
            auto it = find(tmp_player.begin(), tmp_player.end(), nyam);
            MAP[p.first + min_len - x - 1][y] = tmp_MAP[y][x];
            if (MAP[p.first + min_len - x - 1][y] == -1)
                exit_p = {y, x};
            if (it != tmp_player.end()) {
                int dist = distance(tmp_player.begin(), it);
                player[dist].first = p.first + min_len - tmp_player[dist].first - 1;
                player[dist].second = p.second + min_len - tmp_player[dist].second - 1;
            }
        }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    for (int i = 0; i < k; i++) {
        move_player();
        if (player.empty())
            break;
        rotate_MAP();
    }
    cout << ans << endl << exit_p.first << ' ' << exit_p.second;
    return 0;
}