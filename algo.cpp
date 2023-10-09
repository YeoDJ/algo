#include <algorithm>
#include <iostream>
#include <queue>
#include <string>

#define MOVE_Y                                                                                                                                                                                         \
    {                                                                                                                                                                                                  \
        pair<int, int> tmp(y, arr[i].second);                                                                                                                                                          \
        if (MAP[y][arr[i].second] == '#' || (i == 1 && arr[0] == tmp))                                                                                                                                 \
            break;                                                                                                                                                                                     \
        arr[i].first = y;                                                                                                                                                                              \
        if (arr[i] == out)                                                                                                                                                                             \
            break;                                                                                                                                                                                     \
    }
#define MOVE_X                                                                                                                                                                                         \
    {                                                                                                                                                                                                  \
        pair<int, int> tmp(arr[i].first, x);                                                                                                                                                           \
        if (MAP[arr[i].first][x] == '#' || (i == 1 && arr[0] == tmp))                                                                                                                                  \
            break;                                                                                                                                                                                     \
        arr[i].second = x;                                                                                                                                                                             \
        if (arr[i] == out)                                                                                                                                                                             \
            break;                                                                                                                                                                                     \
    }
using namespace std;

int n, m;
bool isExit = false;
vector<string> MAP;
pair<int, int> blue, red, out;
vector<int> dir_path;

void input() {
    cin >> n >> m;
    MAP = vector<string>(n);
    for (int y = 0; y < n; y++) {
        cin >> MAP[y];
        for (int x = 0; x < m; x++) {
            pair<int, int> tmp(y, x);
            (MAP[y][x] == 'B') ? blue = tmp : (MAP[y][x] == 'R') ? red = tmp : (MAP[y][x] == 'O') ? out = tmp : tmp;
        }
    }
}

void move_candies(int dir) {
    // 캔디 순서 선정 후 옮기기, 상하좌우, 각 방향대로 이동하고 있을 때 '#'을 만나면 break
    pair<int, int> arr[2] = {blue, red};
    bool isSwap = false;
    switch (dir) {
    case 0:
        if (isSwap = arr[0].first > arr[1].first)
            swap(arr[0], arr[1]);
        for (int i = 0; i < 2; i++)
            for (int y = arr[i].first - 1; y >= 1; y--)
                MOVE_Y;
        break;
    case 1:
        if (isSwap = arr[0].first < arr[1].first)
            swap(arr[0], arr[1]);
        for (int i = 0; i < 2; i++)
            for (int y = arr[i].first + 1; y < n - 1; y++)
                MOVE_Y;
        break;
    case 2:
        if (isSwap = arr[0].second > arr[1].second)
            swap(arr[0], arr[1]);
        for (int i = 0; i < 2; i++)
            for (int x = arr[i].second - 1; x >= 1; x--)
                MOVE_X;
        break;
    case 3:
        if (isSwap = arr[0].second < arr[1].second)
            swap(arr[0], arr[1]);
        for (int i = 0; i < 2; i++)
            for (int x = arr[i].second + 1; x < m - 1; x++)
                MOVE_X;
        break;
    }
    if (isSwap)
        swap(arr[0], arr[1]);

    // 단, 파랑 사탕이 나가는 경우는 빼야 한다.
    // case 1. 파랑 사탕이 직접 나가는 경우
    if (arr[0] == out)
        return;
    // case 2. 파랑 사탕이 따라 나가는 경우
    int dy[] = {1, -1, 0, 0};
    int dx[] = {0, 0, 1, -1};
    pair<int, int> tmp(arr[1].first + dy[dir], arr[1].second + dx[dir]);
    if (arr[1] == out && arr[0] == tmp)
        return;

    MAP[blue.first][blue.second] = MAP[red.first][red.second] = '.';
    MAP[arr[0].first][arr[0].second] = 'B';
    MAP[arr[1].first][arr[1].second] = 'R';
    blue = arr[0], red = arr[1];
}

void solution(int lvl, int max_lvl, int prev) {
    if (isExit)
        return;

    if (lvl == max_lvl) {
        // 사탕 이동
        vector<string> tmp_MAP = MAP;
        pair<int, int> tmp_blue = blue, tmp_red = red;
        for (auto &&i : dir_path)
            move_candies(i);

        // 빨간색 사탕'만' 통과했는지 따지기
        isExit = red == out;
        MAP = tmp_MAP;
        blue = tmp_blue, red = tmp_red;
        return;
    }

    for (int i = 0; i < 4; i++) {
        if (i == prev)
            continue;
        dir_path.push_back(i);
        solution(lvl + 1, max_lvl, i);
        dir_path.pop_back();
    }
}

int main() {
    // freopen("./input.txt", "r", stdin);
    input();
    int ans;
    for (ans = 1; ans <= 10; ans++) {
        solution(0, ans, -1);
        if (isExit)
            break;
    }

    ans = (!isExit) ? -1 : ans;
    cout << ans;
    return 0;
}
