#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

// �������� -1�� �Ѵ�.
int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, -1, 1};
int n, m, k, ans = 0;
pair<int, int> exit_p;
vector<vector<int>> MAP;
vector<pair<int, int>> player;

// ���� �� & ���� �ƴ϶�� true
bool inRange(int y, int x) { return (0 <= y && y < n && 0 <= x && x < n) && MAP[y][x] <= 0; }

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
    int i = 0, min_len, len, ny, nx;
    pair<int, int> min_p;

    while (i < player.size()) {
        min_len = INT32_MAX;
        min_p = player[i];

        // �ⱸ�� ���� ����� �Ÿ� & �� ��ǥ�� ������ �� �ִ°�?
        for (int dir = 0; dir < 4; dir++) {
            ny = player[i].first + dy[dir];
            nx = player[i].second + dx[dir];
            len = abs(exit_p.first - ny) + abs(exit_p.second - nx);
            min_len = min(min_len, len);
        }

        for (int dir = 0; dir < 4; dir++) {
            ny = player[i].first + dy[dir];
            nx = player[i].second + dx[dir];
            len = abs(exit_p.first - ny) + abs(exit_p.second - nx);
            if (len == min_len && inRange(ny, nx)) {
                min_p = {ny, nx};
                break;
            }
        }

        // player�� �������ٸ�?
        if (min_p != player[i]) {
            ans++;
            if (min_p == exit_p)
                player.erase(player.begin() + i--);
            else
                player[i] = min_p;
        }
        i++;
    }
}

bool compare(pair<int, int> p1, pair<int, int> p2) {
    if (p1.first == p2.first)
        return p1.second < p2.second;
    return p1.first < p2.first;
}

void rotate_MAP() {
    int min_len = n, y, x;
    vector<int> len_arr;
    vector<vector<int>> tmp_MAP = MAP;
    vector<pair<int, int>> tmp_player = player;
    pair<int, int> p = {n, n}, tmp_p, abs_p;

    // �ⱸ�� �������� ������ ũ�⸦ ���Ѵ�.
    for (auto &&i : player)
        len_arr.push_back(max(abs(i.first - exit_p.first) + 1, abs(i.second - exit_p.second) + 1));
    min_len = *min_element(len_arr.begin(), len_arr.end());

    // �»�� ��ǥ�� ���Ѵ�.(y, x �������)
    for (int i = 0; i < player.size(); i++)
        if (len_arr[i] == min_len) {
            // ���� ����(false) or ���� ����(true)
            bool flag = abs(player[i].first - exit_p.first) + 1 == min_len;
            tmp_p = {max(player[i].first, exit_p.first) - min_len + 1, max(player[i].second, exit_p.second) - min_len + 1};
            tmp_p.first = (flag) ? min(player[i].first, exit_p.first) : (tmp_p.first < 0) ? 0 : (tmp_p.first >= n - min_len) ? n - min_len : tmp_p.first;
            tmp_p.second = (!flag) ? min(player[i].second, exit_p.second) : (tmp_p.second < 0) ? 0 : (tmp_p.second >= n - min_len) ? n - min_len : tmp_p.second;
            if (compare(tmp_p, p))
                p = tmp_p;
        }

    // ȸ��(MAP)
    for (y = 0; y < min_len; y++)
        for (x = 0; x < min_len; x++)
            tmp_MAP[x + p.first][p.second + min_len - 1 - y] = MAP[y + p.first][x + p.second];
    MAP = tmp_MAP;

    // �� �� ���
    for (y = p.first; y < p.first + min_len; y++)
        for (x = p.second; x < p.second + min_len; x++) {
            if (MAP[y][x] == -1)
                exit_p = {y, x};
            if (MAP[y][x] > 0)
                MAP[y][x]--;
        }

    // ȸ��(player)
    for (y = 0; y < min_len; y++)
        for (x = 0; x < min_len; x++) {
            tmp_p = {y + p.first, x + p.second};
            for (int i = 0; i < player.size(); i++)
                if (player[i] == tmp_p) {
                    tmp_player[i].first = x + p.first;
                    tmp_player[i].second = p.second + min_len - 1 - y;
                }
        }
    player = tmp_player;
}

int main() {
    input();
    return 0;
}