#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

// ��� ����(t), ��ǥ(y, x)
struct block {
    int t, y, x;
};

int k;
vector<vector<bool>> MAP(10, vector<bool>());
vector<block> arr;

void input() {
    cin >> k;
    for (int y = 0; y < 10; y++) {
        int sx = (y <= 3) ? 10 : 4;
        for (int x = 0; x < sx; x++)
            MAP[y].push_back(false);
    }

    arr = vector<block>(k);
    for (auto &&i : arr)
        cin >> i.t >> i.y >> i.x;
}

void setting_MAP(block b, bool isFill) {
    MAP[b.y][b.x] = isFill;
    (b.t == 2) ? MAP[b.y][b.x + 1] = isFill : (b.t == 3) ? MAP[b.y + 1][b.x] = isFill : 1;
}

void tetris_down(block b) {
    // ������ ����� ��� �� ���ΰ�?
    pair<int, int> p(4, b.x);
    for (p.first = 4; p.first < 10; p.first++)
        if (MAP[p.first][p.second]) {
            p.first--;
            break;
        }
    (p.first == 10) ? p.first-- : 1;

    // ��� ������ 2��� �߰������� Ȯ�� ������ �ʿ��ϴ�.
    if (b.t == 2) {
        pair<int, int> tmp(4, b.x + 1);
        for (tmp.first = 4; tmp.first < 10; tmp.first++)
            if (MAP[tmp.first][tmp.second]) {
                tmp.first--;
                break;
            }
        (tmp.first == 10) ? tmp.first-- : 1;
        p.first = min(p.first, tmp.first);
    }

    // ��� ������ ���� �߰� ����� �ڵ��� �д�.
    MAP[p.first][p.second] = true;
    (b.t == 2) ? MAP[p.first][p.second + 1] = true : (b.t == 3) ? MAP[p.first - 1][p.second] = true : 1;
}

void tetris_right(block b) {
    // ������ ����� ��� �� ���ΰ�?
    pair<int, int> p(b.y, 4);
    for (p.second = 4; p.second < 10; p.second++)
        if (MAP[p.first][p.second]) {
            p.second--;
            break;
        }
    (p.second == 10) ? p.second-- : 1;

    // ��� ������ 3�̶�� �߰������� Ȯ�� ������ �ʿ��ϴ�.
    if (b.t == 3) {
        pair<int, int> tmp(b.y + 1, 4);
        for (tmp.second = 4; tmp.second < 10; tmp.second++)
            if (MAP[tmp.first][tmp.second]) {
                tmp.second--;
                break;
            }
        (tmp.second == 10) ? tmp.second-- : 1;
        p.second = min(p.second, tmp.second);
    }

    // ��� ������ ���� �߰� ����� �ڵ��� �д�.
    MAP[p.first][p.second] = true;
    (b.t == 2) ? MAP[p.first][p.second - 1] = true : (b.t == 3) ? MAP[p.first + 1][p.second] = true : 1;
}

int break_down() {
    // ���� �� ����
    int ans = 0;
    vector<bool> isBreak(4, true);
    for (int y = 6; y < 10; y++)
        for (int x = 0; x < 4; x++)
            if (!MAP[y][x]) {
                isBreak[y - 6] = false;
                break;
            }

    // ���ְ� ��� ������
    vector<vector<bool>> tmp_MAP(4, vector<bool>(4, false));
    int ny = 3;
    for (int y = 9; y >= 6; y--)
        if (!isBreak[y - 6])
            tmp_MAP[ny--] = MAP[y];
    if (isBreak[0])
        for (int x = 0; x < 4; x++)
            if (MAP[5][x]) {
                MAP[5][x] = false;
                tmp_MAP[0][x] = true;
            }
    for (int y = 6; y < 10; y++)
        MAP[y] = tmp_MAP[y - 6];

    // ���� ��Ͽ� ���� ó��(�� ��° ����� ä���� �ִ°�?)
    ny = 6;
    for (int y = 4; y <= 5; y++) {
        for (int x = 0; x < 4; x++)
            if (MAP[y][x]) {
                ny = y;
                break;
            }
        if (ny == 4)
            break;
    }

    // ���� ��Ͽ� ���� ó��(�׸�ŭ ��� ������)
    for (int y = ny; y < ny + 4; y++)
        tmp_MAP[y - ny] = MAP[y];
    MAP[4] = MAP[5] = {0, 0, 0, 0};
    for (int y = 6; y < 10; y++)
        MAP[y] = tmp_MAP[y - 6];

    // ���� �ű��
    for (auto &&i : isBreak)
        ans += i;
    return ans;
}

int break_right() {
    // ���� �� ����
    int ans = 0;
    vector<bool> isBreak(4, true);
    for (int x = 6; x < 10; x++)
        for (int y = 0; y < 4; y++)
            if (!MAP[y][x]) {
                isBreak[x - 6] = false;
                break;
            }

    // ���ְ� ��� ������
    vector<vector<bool>> tmp_MAP(4, vector<bool>(4, false));
    int nx = 3;
    for (int x = 9; x >= 6; x--)
        if (!isBreak[x - 6]) {
            for (int y = 0; y < 4; y++)
                tmp_MAP[y][nx] = MAP[y][x];
            nx--;
        }
    if (isBreak[0])
        for (int y = 0; y < 4; y++)
            if (MAP[y][5]) {
                MAP[y][5] = false;
                tmp_MAP[y][0] = true;
            }
    for (int x = 6; x < 10; x++)
        for (int y = 0; y < 4; y++)
            MAP[y][x] = tmp_MAP[y][x - 6];

    // ���� ��Ͽ� ���� ó��(�� ��° ������ ä���� �ִ°�?)
    nx = 6;
    for (int x = 4; x <= 5; x++) {
        for (int y = 0; y < 4; y++)
            if (MAP[y][x]) {
                nx = x;
                break;
            }
        if (nx == 4)
            break;
    }

    // ���� ��Ͽ� ���� ó��(�׸�ŭ ��� ������)
    for (int x = nx; x < nx + 4; x++)
        for (int y = 0; y < 4; y++)
            tmp_MAP[y][x - nx] = MAP[y][x];
    for (int y = 0; y < 4; y++)
        MAP[y][4] = MAP[y][5] = 0;
    for (int x = 6; x < 10; x++)
        for (int y = 0; y < 4; y++)
            MAP[y][x] = tmp_MAP[y][x - 6];

    // ���� �ű��
    for (auto &&i : isBreak)
        ans += i;
    return ans;
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();
    int ans = 0, sum = 0;

    for (int i = 0; i < k; i++) {
        setting_MAP(arr[i], true);
        tetris_down(arr[i]);
        tetris_right(arr[i]);
        ans += break_down() + break_right();
        setting_MAP(arr[i], false);
    }

    for (int y = 6; y < 10; y++)
        for (int x = 0; x < 4; x++) {
            sum += MAP[y][x];
            sum += MAP[x][y];
        }
    cout << ans << endl << sum;
    return 0;
}