#include <iostream>
#include <queue>
using namespace std;

struct mold {
    int len, dir, size;
};
int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, 1, -1};
int n, m, k;
vector<vector<mold>> MAP;

void input() {
    cin >> n >> m >> k;
    MAP = vector<vector<mold>>(n, vector<mold>(m));
    for (int i = 0; i < k; i++) {
        int x, y, s, d, b;
        cin >> x >> y >> s >> d >> b;
        MAP[y - 1][x - 1] = {s, d - 1, b};
    }
}

int main() {
    input();
    return 0;
}