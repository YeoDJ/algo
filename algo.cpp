#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

struct mold {
    int y, x;
    int vel, dir, sz;
};

int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, 1, -1};
int n, m, k, ans = 0;
vector<mold> arr;

bool compare(mold m1, mold m2) {
    if (m1.y == m2.y) {
        if (m1.x == m2.x)
            return m1.sz < m2.sz;
        return m1.x < m2.x;
    }
    return m1.y < m2.y;
}

void input() {
    cin >> n >> m >> k;
    for (int i = 0; i < k; i++) {
        int y, x, s, d, b;
        cin >> y >> x >> s >> d >> b;
        arr.push_back({y - 1, x - 1, s, d - 1, b});
    }
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();

    return 0;
}