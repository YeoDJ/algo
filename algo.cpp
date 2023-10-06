#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>
#define FIRST mal[path[i]].first
#define SECOND mal[path[i]].second
using namespace std;

int ans = 0;
int arr[10], path[10];
vector<vector<int>> MAP(4);

void input() {
    int idx = 1;
    vector<int> tmp[3] = {{13, 16, 19}, {22, 24}, {28, 27, 26}};
    memset(path, -1, sizeof(path));
    for (auto &&i : arr)
        cin >> i;

    for (int i = 2; i <= 40; i += 2) {
        MAP[0].push_back(i);
        for (int j = idx; j <= 3; j++)
            MAP[j].push_back(i);
        if (!(i % 10))
            idx++;
    }

    for (int i = 0; i < 3; i++)
        for (auto &&j : tmp[i])
            MAP[i + 1].push_back(j);
    for (int i = 25; i <= 40; i += 5)
        for (int j = 1; j <= 3; j++)
            MAP[j].push_back(i);
}

int find_dist() {
    int sum = 0, num;
    // (경로, 몇 번째), 출발: -1, 도착: 99
    vector<pair<int, int>> mal(4, {0, -1});

    for (int i = 0; i < 10; i++)
        if (SECOND != 99) {
            SECOND += arr[i];
            if (SECOND >= MAP[FIRST].size())
                SECOND = 99;
            else {
                num = MAP[FIRST][SECOND];
                sum += num;
                if (!FIRST && !(num % 10) && num != 40)
                    FIRST = num / 10;

                // 겹쳐 있다면 return
                for (int j = 0; j < 4; j++)
                    if (j != path[i] && mal[j].second != -1 && mal[j].second != 99) {
                        if (mal[path[i]] == mal[j])
                            return 0;
                        for (int k = 25; k <= 40; k += 5)
                            if (MAP[FIRST][SECOND] == k && MAP[mal[j].first][mal[j].second] == k)
                                return 0;
                    }
            }
        }

    return sum;
}

void solution(int lvl) {
    if (lvl == 10) {
        ans = max(ans, find_dist());
        if (ans == 230) {
            find_dist();
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        path[lvl] = i;
        solution(lvl + 1);
        path[lvl] = -1;
    }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();
    solution(0);
    cout << ans;
    return 0;
}