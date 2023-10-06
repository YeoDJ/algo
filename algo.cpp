#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

/*
    1. ���� �ִ� �°� �� ���� ���� ��ġ���� ���� ����� �°� ����
    2. ������ �°����� �̵�
    3. �ű⼭ �� ���������� �ִܰŸ� ���
    4. ���������� �̵�
    5. 4�� �������� �Ҹ��� ���� x 2��ŭ ����, �� �������� �� ���� == 0�̾ ���� ����, ������ �°����� ����
    6. � ��ε� ���� ���߿� ���Ḧ ��� �Ҹ��Ѵٰ� �ǴܵǸ� return
*/

struct customer {
    pair<int, int> start, end;
    bool isArrive;
};

int n, m, c;
vector<vector<int>> MAP;
pair<int, int> car;
vector<customer> arr;

bool inRange(int y, int x) { return 0 <= y && y < n && 0 <= x && x < n && !MAP[y][x]; }

bool compare(customer c1, customer c2) {
    if (c1.start.first == c2.start.first)
        return c1.start.second < c2.start.second;
    return c1.start.first < c2.start.first;
}

bool isEmpty() {
    for (auto &&i : arr)
        if (!i.isArrive)
            return true;
    return false;
}

void input() {
    cin >> n >> m >> c;
    MAP = vector<vector<int>>(n, vector<int>(n));
    arr = vector<customer>(m);

    for (int i = 0; i < n; i++)
        for (auto &&j : MAP[i])
            cin >> j;
    cin >> car.first >> car.second;
    car.first--, car.second--;

    for (auto &&i : arr) {
        cin >> i.start.first >> i.start.second >> i.end.first >> i.end.second;
        i.start.first--, i.start.second--;
        i.end.first--, i.end.second--;
    }
}

int bfs(customer man, bool isMove) {
    int dy[] = {-1, 1, 0, 0};
    int dx[] = {0, 0, -1, 1};

    queue<pair<int, int>> q;
    vector<vector<int>> used = MAP;
    vector<vector<int>> dist(n, vector<int>(n, 0));
    q.push(car);
    used[car.first][car.second] = 1;

    while (!q.empty()) {
        pair<int, int> cur = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int y = cur.first + dy[i];
            int x = cur.second + dx[i];
            if (inRange(y, x) && !used[y][x]) {
                q.push({y, x});
                used[y][x] = 1;
                dist[y][x] = dist[cur.first][cur.second] + 1;
            }

            pair<int, int> tmp(y, x);
            if ((!isMove && man.start == tmp) || (isMove && man.end == tmp))
                return dist[y][x];
        }
    }
    return n * n;
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    while (isEmpty()) {
        // 1. ���� �ִ� �°� �� ���� ���� ��ġ���� ���� ����� �°� ����
        sort(arr.begin(), arr.end(), compare);
        int min_idx = -1, min_dist = n * n;
        for (int i = 0; i < m; i++)
            if (!arr[i].isArrive) {
                int dist = bfs(arr[i], false);
                if (dist < min_dist) {
                    min_dist = dist;
                    min_idx = i;
                }
            }

        // �� �������� ���Ḧ ��� �Ҹ��ϰų� �� �� �ִ� ���� ���ٸ� break(�� ���� 0�� ����)
        c -= min_dist;
        if (min_dist == n * n || c <= 0)
            break;

        // 2. ������ �°����� �̵�
        car = arr[min_idx].start;

        // 3. �ű⼭ �� ���������� �ִܰŸ� ���, �� �� �ִ� ���� ���ٸ� break(�� ���� 0 ��)
        min_dist = bfs(arr[min_idx], true);
        c -= min_dist;
        if (min_dist == n * n || c < 0)
            break;

        // 4. �� �� ���������� �̵�, �Ҹ��� ���� x 2��ŭ ����, ������ �°����� ����
        car = arr[min_idx].end;
        arr[min_idx].isArrive = true;
        c += min_dist * 2;
    }

    c = (c <= 0 || isEmpty()) ? -1 : c;
    cout << c;
    return 0;
}