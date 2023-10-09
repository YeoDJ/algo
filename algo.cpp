#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;

int ans, sum;
int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dx[] = {0, -1, -1, -1, 0, 1, 1, 1};
// (도둑 번호, 방향), 도둑 번호가 음수라면 잡혔다는 의미
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

// 경찰 말이 파라미터로 주어질 때 도둑 말은 어떻게 움직이는가?
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

// 경찰 말을 어떻게 움직일 것인가?
void solution(int ny, int nx, int dir) {
    pair<int, int> tmp_MAP[4][4];
    bool flag = false;

    while (1) {
        // 도둑 움직이기
        for (int i = 1; i <= 16; i++) {
            flag = false;
            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++)
                    if (abs(MAP[y][x].first) == i) {
                        // 경찰은 지금 움직이면 안됨
                        if (MAP[y][x].first > 0)
                            move_thief({y, x}, {ny, nx}, MAP[y][x].second);
                        flag = true;
                        break;
                    }
                if (flag)
                    break;
            }
        }

        // 경찰 움직이기
        memcpy(tmp_MAP, MAP, sizeof(MAP));
        while (1) {
            ny += dy[dir], nx += dx[dir];
            if (MAP[ny][nx].first < 0)
                continue;
            if (flag = !inRange(ny, nx))
                break;

            // 점수 구하기
            sum += MAP[ny][nx].first;
            MAP[ny][nx].first *= -1;
            ans = max(ans, sum);
            solution(ny, nx, MAP[ny][nx].second);

            // 경찰 되돌리기
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