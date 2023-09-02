#include <cstring>
#include <iostream>
#include <queue>
using namespace std;

int n, dir = 2, amount = 0, ans = 0;
int dy[] = {0, -1, 0, 1};
int dx[] = {1, 0, -1, 0};
pair<int, int> cur;
vector<vector<int>> MAP;

bool inRange(pair<int, int> p) { return 0 <= p.first && p.first < n && 0 <= p.second && p.second < n; }

void input() {
    cin >> n;
    MAP = vector<vector<int>>(n, vector<int>(n));
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++)
            cin >> MAP[y][x];
    cur = {n / 2, n / 2};
}

void process() {
    // 1. 이동할 먼지 비율 기준정보 설정
    int std_dust[5][5] = {{0, 0, 2, 0, 0}, {0, 10, 7, 1, 0}, {5, -1, 0, 0, 0}, {0, 10, 7, 1, 0}, {0, 0, 2, 0, 0}};
    int dust_arr[5][5];
    int turn = (dir <= 1) ? dir + 2 : dir - 2;
    memcpy(dust_arr, std_dust, sizeof(std_dust));

    for (int i = 0; i < turn; i++) {
        for (int y = 0; y < 5; y++)
            for (int x = 0; x < 5; x++)
                dust_arr[4 - x][y] = std_dust[y][x];
        memcpy(std_dust, dust_arr, sizeof(dust_arr));
    }

    // 2. 없어질 먼지 대상 선정 및 이동한 먼지 정보
    int cur_dust = MAP[cur.first][cur.second];
    int mv_dust;
    for (int y = -2; y <= 2; y++)
        for (int x = -2; x <= 2; x++)
            if (dust_arr[y + 2][x + 2] > 0) {
                mv_dust = int(float(MAP[cur.first][cur.second]) * (float(dust_arr[y + 2][x + 2]) / 100));
                cur_dust -= int(mv_dust);
                (!inRange({cur.first + y, cur.second + x})) ? ans += mv_dust : MAP[cur.first + y][cur.second + x] += mv_dust;
            }

    // 3. a %에 대한 처리
    bool flag = false;
    for (int y = -2; y <= 2; y++) {
        for (int x = -2; x <= 2; x++) {
            flag = dust_arr[y + 2][x + 2] == -1;
            if (flag) {
                (!inRange({cur.first + y, cur.second + x})) ? ans += cur_dust : MAP[cur.first + y][cur.second + x] += cur_dust;
                break;
            }
        }
        if (flag)
            break;
    }
    MAP[cur.first][cur.second] = 0;
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    while (inRange(cur)) {
        if (!(dir % 2))
            amount++;
        for (int i = 0; i < amount; i++) {
            cur.first += dy[dir], cur.second += dx[dir];
            if (inRange(cur))
                process();
        }
        dir = (dir == 3) ? 0 : dir + 1;
    }

    cout << ans;
    return 0;
}