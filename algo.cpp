#include <algorithm>
#include <iostream>
#include <queue>
#define DIST distance(red_bomb.begin(), find(red_bomb.begin(), red_bomb.end(), tmp))
using namespace std;

// -1: �浹, 0: ����, 1 ~ m: �� �̿�, -2: �� ����
// red_cnt: ���� �� ������ź ����, std_p: ������
int n, m;
int red_cnt = 0, tmp_red_cnt;
pair<int, int> std_p, tmp_std_p;
vector<vector<int>> MAP, used, bundle, tmp_bundle;
vector<pair<int, int>> red_bomb;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n; }

void input() {
    cin >> n >> m;
    MAP = vector<vector<int>>(n, vector<int>(n));
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++) {
            cin >> MAP[y][x];
            if (MAP[y][x] == 0)
                red_bomb.push_back({y, x});
        }
}

// ���� ũ��
int bfs(int ny, int nx) {
    tmp_bundle = vector<vector<int>>(n, vector<int>(n, 0));
    tmp_red_cnt = 0;
    tmp_std_p = {-1, n};
    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};
    int cnt = 1;

    queue<pair<int, int>> q;
    q.push({ny, nx});
    used[ny][nx] = 1;

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        tmp_bundle[cur.first][cur.second] = 1;
        tmp_red_cnt += (find(red_bomb.begin(), red_bomb.end(), cur) != red_bomb.end()) ? 1 : 0;
        tmp_std_p = (MAP[cur.first][cur.second] > 0 && (cur.first > tmp_std_p.first || (cur.first == tmp_std_p.first && cur.second < tmp_std_p.second))) ? cur : tmp_std_p;
        q.pop();

        for (int i = 0; i < 4; i++) {
            int y = cur.first + dy[i];
            int x = cur.second + dx[i];
            if (inRange(y, x) && !used[y][x] && (MAP[y][x] == MAP[ny][nx] || !MAP[y][x])) {
                used[y][x] = 1;
                cnt++;
                q.push({y, x});
            }
        }
    }
    return cnt;
}

bool compare(int sz1, int sz2) {
    if (sz1 == sz2) {
        if (tmp_red_cnt == red_cnt) {
            if (tmp_std_p.first == std_p.first)
                return tmp_std_p.second < std_p.second;
            return tmp_std_p.first > std_p.first;
        }
        return tmp_red_cnt < red_cnt;
    }
    return sz1 > sz2;
}

bool isBomb() {
    int max_sz = 0;
    used = vector<vector<int>>(n, vector<int>(n, 0));
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            if (MAP[y][x] < 0)
                used[y][x] = 1;

    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            if (!used[y][x] && MAP[y][x] > 0) {
                int sz = bfs(y, x);
                if (compare(sz, max_sz)) {
                    max_sz = sz;
                    red_cnt = tmp_red_cnt;
                    std_p = tmp_std_p;
                    bundle = tmp_bundle;
                }
                for (auto &&i : red_bomb)
                    used[i.first][i.second] = 0;
            }
    return max_sz > 1;
}

// �� �� ��
int bomb() {
    int cnt = 0;
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            if (bundle[y][x]) {
                cnt++;
                pair<int, int> tmp(y, x);
                if (!MAP[y][x])
                    red_bomb.erase(red_bomb.begin() + DIST);
                MAP[y][x] = -2;
            }
    return cnt * cnt;
}

// �� ��� ��
void drop() {
    vector<vector<int>> tmp_MAP(n, vector<int>(n, -2));
    for (int x = 0; x < n; x++) {
        int ny = n - 1;
        for (int y = n - 1; y >= 0; y--) {
            if (MAP[y][x] >= 0) {
                tmp_MAP[ny][x] = MAP[y][x];
                pair<int, int> tmp(y, x);
                if (!MAP[y][x])
                    red_bomb[DIST] = {ny, x};
                ny--;
            } else if (MAP[y][x] == -1) {
                tmp_MAP[y][x] = -1;
                ny = y - 1;
            }
        }
    }
    MAP = tmp_MAP;
}

// ȸ ? ��
void rotate() {
    vector<vector<int>> tmp_MAP(n, vector<int>(n, -2));
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++) {
            tmp_MAP[n - 1 - x][y] = MAP[y][x];
            pair<int, int> tmp(y, x);
            if (!MAP[y][x])
                red_bomb[DIST] = {n - 1 - x, y};
        }
    MAP = tmp_MAP;
}

int main() {
    input();
    int ans = 0;

    while (isBomb()) {
        ans += bomb();
        drop();
        rotate();
        drop();
    }

    cout << ans;
    return 0;
}