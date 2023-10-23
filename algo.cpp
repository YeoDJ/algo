#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;

typedef pair<int, int> p;
struct compare {
    bool operator()(p p1, p p2) { return p1.first + p1.second > p2.first + p2.second; }
};

int n, m, k;
vector<int> arr1;
priority_queue<int, vector<int>, greater<int>> arr2;

void input() {
    int num;
    cin >> n >> m >> k;
    arr1 = vector<int>(n);
    for (auto &&i : arr1)
        cin >> i;
    for (int i = 0; i < m; i++) {
        int num;
        cin >> num;
        arr2.push(num);
    }
    sort(arr1.begin(), arr1.end());
}

int main() {
    freopen("./input.txt", "r", stdin);
    input();
    priority_queue<p, vector<p>, compare> pq;

    while (k--) {
        int min_num = arr2.top();
        arr2.pop();
        for (auto &&i : arr1)
            pq.push({i, min_num});
        if (!k)
            cout << pq.top().first + pq.top().second;
        pq.pop();
    }
    return 0;
}