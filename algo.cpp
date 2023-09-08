#include <iostream>
#include <queue>
using namespace std;

int n, m, q, a, d, k;
vector<vector<int>> MAP;

void input() {
    cin >> n >> m >> q;
    MAP = vector<vector<int>>(n, vector<int>(m));
    for (int y = 0; y < n; y++)
        for (int x = 0; x < m; x++)
            cin >> MAP[y][x];
}

void rotate(int plate, int dir, int cnt) {
    for (int i = 0; i < cnt; i++)
        for (int j = 0; j < m - 1; j++)
            (dir) ? swap(MAP[plate][j], MAP[plate][j + 1]) : swap(MAP[plate][m - 1 - j], MAP[plate][m - 2 - j]);
}

bool isSame(int y, int x) {
    if (!MAP[y][x])
        return false;

    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};
    for (int i = 0; i < 4; i++) {
        int ny = y + dy[i];
        int nx = x + dx[i];
        nx = (nx < 0) ? m - 1 : (nx >= m) ? 0 : nx;
        if (0 <= ny && ny < n && MAP[y][x] == MAP[ny][nx])
            return true;
    }
    return false;
}

vector<vector<bool>> erase_num() {
    int avg = 0, cnt = 0;
    bool isErase = false;
    vector<vector<bool>> erase_MAP(n, vector<bool>(m, false));

    // 1. ???? ??? ????(???? ???? ???)
    for (int y = 0; y < n; y++)
        for (int x = 0; x < m; x++) {
            if (isSame(y, x))
                erase_MAP[y][x] = isErase = true;
            else if (MAP[y][x]) {
                avg += MAP[y][x];
                cnt++;
            }
        }

    if (cnt)
        avg /= cnt;
    for (int y = 0; y < n; y++)
        for (int x = 0; x < m; x++)
            // 2. ?????
            if (erase_MAP[y][x])
                MAP[y][x] = 0;
            // 3. ???????? ???? ???? ??
            else if (!isErase && MAP[y][x])
                (MAP[y][x] > avg) ? MAP[y][x]-- : (MAP[y][x] < avg) ? MAP[y][x]++ : 1;
    return v;
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    for (int i = 0; i < q; i++) {
        cin >> a >> d >> k;
        for (int j = a - 1; j < n; j += a)
            rotate(j, d, k);
        exist_MAP = erase_num();
    }

    int ans = 0;
    for (int y = 0; y < n; y++)
        for (int x = 0; x < m; x++)
            ans += MAP[y][x];
    cout << ans;
    return 0;
}