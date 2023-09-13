#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dx[] = {0, 1, 1, 1, 0, -1, -1, -1};
int n, m, k;
vector<vector<int>> food, plus_food;
vector<vector<vector<int>>> virus;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n; }

void input() {
    cin >> n >> m >> k;
    food = vector<vector<int>>(n, vector<int>(n, 5));
    plus_food = food;
    virus = vector<vector<vector<int>>>(n, vector<vector<int>>(n));

    for (int i = 0; i < n; i++)
        for (auto &&j : plus_food[i])
            cin >> j;
    for (int i = 0; i < m; i++) {
        int y, x, age;
        cin >> y >> x >> age;
        virus[y - 1][x - 1].push_back(age);
    }
}

void sort_virus() {
    // virus[0]�� ���� ���� ���� ���̷���
    for (int i = 0; i < n; i++)
        for (auto &&j : virus[i])
            if (j.size())
                sort(j.begin(), j.end());
}

void food_algo() {
    for (int y = 0; y < n; y++)
        for (int x = 0; x < n; x++) {
            int i, sz = virus[y][x].size();
            // � ���̷������� ��� ����, ���� ���̸�ŭ �Ա�
            for (i = 0; i < sz; i++)
                if (virus[y][x][i] <= food[y][x])
                    food[y][x] -= virus[y][x][i]++;
                else
                    break;

            // ���� ���̷��� / 2��ŭ ��� �߰� �� �Ҹ�
            for (int j = sz - 1; j >= i; j--) {
                food[y][x] += virus[y][x][j] / 2;
                virus[y][x].pop_back();
            }
        }
}

void virus_algo(int y, int x) {
    for (int i = 0; i < 8; i++) {
        int ny = y + dy[i];
        int nx = x + dx[i];
        if (inRange(ny, nx))
            virus[ny][nx].push_back(1);
    }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    for (int i = 0; i < k; i++) {
        sort_virus();
        food_algo();

        // ����
        for (int y = 0; y < n; y++)
            for (int x = 0; x < n; x++)
                for (int i = 0; i < virus[y][x].size(); i++)
                    if (!(virus[y][x][i] % 5))
                        virus_algo(y, x);

        // ��� �߰�
        for (int y = 0; y < n; y++)
            for (int x = 0; x < n; x++)
                food[y][x] += plus_food[y][x];
    }

    int ans = 0;
    for (int i = 0; i < n; i++)
        for (auto &&j : virus[i])
            ans += j.size();
    cout << ans;
    return 0;
}