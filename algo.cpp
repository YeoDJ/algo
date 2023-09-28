#include <iostream>
#include <queue>
using namespace std;

int n, k;

vector<vector<char>> MAP;

bool input() {
    cin >> n;
    if (n < 1 || n > 100 || !(n % 2)) {
        cout << "INPUT ERROR!";
        return false;
    }
    k = n / 2 + 1;
    MAP = vector<vector<char>>(n, vector<char>(n, ' '));
    return true;
}

void solution() {
    int cnt = 1;
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < cnt; j++)
            MAP[i][j] = MAP[n - 1 - i][j] = '*';
        cnt += 2;
    }
}

int main() {
    if (!input())
        return 0;

    solution();
    for (int i = 0; i < n; i++) {
        int cnt = (i < k) ? i : n - 1 - i;
        for (int j = 0; j < cnt; j++)
            cout << ' ';
        for (auto &&i : MAP[i])
            cout << i;
        cout << endl;
    }

    return 0;
}