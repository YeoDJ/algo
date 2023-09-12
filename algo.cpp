#include <iostream>
#include <queue>
#include <tuple>
using namespace std;

struct mold {
    int vel, dir, sz;
};
int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, 1, -1};
int n, m, k, ans = 0;
mold md_init({-1, -1, -1});
vector<vector<vector<mold>>> MAP;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < m; }

void input() {
    cin >> n >> m >> k;
    MAP = vector<vector<vector<mold>>>(n, vector<vector<mold>>(m));
    for (int i = 0; i < k; i++) {
        int x, y, s, d, b;
        cin >> x >> y >> s >> d >> b;
        MAP[x - 1][y - 1].push_back({s, d - 1, b});
    }
}

mold findMaxSize(int y, int x) {
    mold md = MAP[y][x][0];
    for (auto &&i : MAP[y][x])
        if (md.sz < i.sz)
            md = i;
    return md;
}

void solution() {
    int y, x;
    vector<vector<vector<mold>>> tmp_MAP(n, vector<vector<mold>>(m));

    // 2. 이동하기
    for (y = 0; y < n; y++)
        for (x = 0; x < m; x++)
            if (MAP[y][x].size()) {
                int ey = y, ex = x;
                for (int i = 0; i < MAP[y][x][0].vel; i++) {
                    ey += dy[MAP[y][x][0].dir];
                    ex += dx[MAP[y][x][0].dir];
                    if (!inRange(ey, ex)) {
                        MAP[y][x][0].dir += (MAP[y][x][0].dir % 2) ? -1 : 1;
                        ey += dy[MAP[y][x][0].dir] * 2;
                        ex += dx[MAP[y][x][0].dir] * 2;
                    }
                }
                tmp_MAP[ey][ex].push_back(MAP[y][x][0]);
                MAP[y][x].pop_back();
            }
    MAP = tmp_MAP;

    // 3. 잡아먹기
    for (y = 0; y < n; y++)
        for (x = 0; x < m; x++)
            if (MAP[y][x].size() > 1) {
                mold target = findMaxSize(y, x);
                MAP[y][x].clear();
                MAP[y][x].push_back(target);
            }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    for (int x = 0; x < m; x++) {
        for (int y = 0; y < n; y++)
            if (MAP[y][x].size()) {
                // 1. 채취
                ans += MAP[y][x][0].sz;
                MAP[y][x].pop_back();
                break;
            }
        solution();
    }

    cout << ans;
    return 0;
}