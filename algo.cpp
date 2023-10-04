#include <iostream>
#include <queue>
using namespace std;

int n, m, k;
vector<vector<char>> MAP;

bool input() {
    cin >> n >> m;
    if (n < 1 || n > 100 || m < 1 || m > 4)
        return false;
    k = (m < 3) ? n / 2 + 1 : n;
    MAP = vector<vector<char>>(n, vector<char>(k, ' '));
    return true;
}

void solution() {
    switch (m) {
    case 1:
        break;
    case 2:
        /* code */
        break;
    case 3:
        /* code */
        break;
    case 4:
        /* code */
        break;
    }
}

int main() {
    if (!input()) {
        cout << "INPUT ERROR!";
        return 0;
    }

    solution();
    for (int i = 0; i < n; i++) {
        for (auto &&j : MAP[i])
            cout << j;
        cout << endl;
    }

    return 0;
}