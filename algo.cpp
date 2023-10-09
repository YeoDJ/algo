#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;

int ans, sum;
int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dx[] = {0, -1, -1, -1, 0, 1, 1, 1};
// (���� ��ȣ, ����), ���� ��ȣ�� ������� �����ٴ� �ǹ�
pair<int, int> MAP[4][4];

bool inRange(int y, int x) { return 0 <= y && y < 4 && 0 <= x && x < 4; }

int input() {
    for (int i = 0; i < 4; i++)
        for (auto &&j : MAP[i]) {
            cin >> j.first >> j.second;
            j.second--;
        }

    ans = sum = MAP[0][0].first;
    MAP[0][0].first *= -1;
    return MAP[0][0].second;
}

// ���� ���� �Ķ���ͷ� �־��� �� ���� ���� ��� �����̴°�?
void move_thief(pair<int, int> thief, pair<int, int> police, int &dir) {
    for (int i = 0; i < 8; i++) {
        pair<int, int> p(thief.first + dy[dir], thief.second + dx[dir]);
        if (inRange(p.first, p.second) && p != police) {
            swap(MAP[thief.first][thief.second], MAP[p.first][p.second]);
            break;
        }
        dir = (dir == 7) ? 0 : dir + 1;
    }
}

// ���� ���� ��� ������ ���ΰ�?
void solution(int ny, int nx, int dir) {
    pair<int, int> tmp_MAP[4][4];
    bool flag = false;

    while (1) {
        // ���� �����̱�
        for (int i = 1; i <= 16; i++) {
            flag = false;
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++)
                    if (abs(MAP[y][x].first) == i) {
                        // ������ ���� �����̸� �ȵ�
                        if (MAP[y][x].first > 0)
                            move_thief({y, x}, {ny, nx}, MAP[y][x].second);
                        flag = true;
                        break;
                    }
                if (flag)
                    break;
            }
        }

        // ���� �����̱�
        memcpy(tmp_MAP, MAP, sizeof(MAP));
        while (1) {
            ny += dy[dir], nx += dx[dir];
            if (MAP[ny][nx].first < 0)
                continue;
            if (flag = !inRange(ny, nx))
                break;

            // ���� ���ϱ�
            sum += MAP[ny][nx].first;
            MAP[ny][nx].first *= -1;
            ans = max(ans, sum);
            solution(ny, nx, MAP[ny][nx].second);

            // ���� �ǵ�����
            sum += MAP[ny][nx].first;
            MAP[ny][nx].first *= -1;
            memcpy(MAP, tmp_MAP, sizeof(tmp_MAP));
        }
        if (flag)
            return;
    }
}

int main() {
    solution(0, 0, input());
    cout << ans;
    return 0;
}