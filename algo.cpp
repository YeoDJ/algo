#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
using namespace std;

int n, m;
vector<vector<int>> MAP;
// 해당 box 번호가 어느 벨트에 있는지 저장
unordered_map<int, pair<int, int>> box_to_pos;

void input() {
    cin >> n >> m;
    MAP = vector<vector<int>>(n, vector<int>());
    for (int i = 0; i < m; i++) {
        int num;
        cin >> num;
        MAP[num - 1].push_back(i + 1);
        box_to_pos[i + 1] = {num - 1, MAP[num - 1].size() - 1};
    }
}

int move_to_all(int src, int dst) {
    int sz = MAP[src].size();
    if (!sz)
        return MAP[dst].size();

    // 선물 상자 옮기기
    for (int i = 0; i < sz; i++) {
        MAP[dst].insert(MAP[dst].begin(), MAP[src][MAP[src].size() - 1]);
        MAP[src].pop_back();
    }

    // box_to_pos 업데이트
    for (int i = 0; i < MAP[dst].size(); i++)
        box_to_pos[MAP[dst][i]] = {dst, i};
    return MAP[dst].size();
}

int change_to_front(int src, int dst) {
    int ssz = MAP[src].size(), dsz = MAP[dst].size();
    if (!ssz && !dsz)
        return MAP[dst].size();

    // 선물 상자를 먼저 옮긴 뒤 box_to_pos 업데이트
    if (!ssz) {
        MAP[src].push_back(MAP[dst][0]);
        MAP[dst].erase(MAP[dst].begin());
        box_to_pos[MAP[src][0]] = {src, 0};
        for (int i = 0; i < dsz - 1; i++)
            box_to_pos[MAP[dst][i]] = {dst, i};
        dsz--;
    } else if (!dsz) {
        MAP[dst].push_back(MAP[src][0]);
        MAP[src].erase(MAP[src].begin());
        box_to_pos[MAP[dst][0]] = {dst, 0};
        for (int i = 0; i < ssz - 1; i++)
            box_to_pos[MAP[src][i]] = {src, i};
        dsz++;
    } else {
        swap(MAP[src][0], MAP[dst][0]);
        box_to_pos[MAP[src][0]] = {src, 0};
        box_to_pos[MAP[dst][0]] = {dst, 0};
    }
    return dsz;
}

int divide_present(int src, int dst) {
    int sz = MAP[src].size() / 2;
    if (!sz)
        return MAP[dst].size();

    // 선물 상자 옮기기
    for (int i = sz - 1; i >= 0; i--) {
        MAP[dst].insert(MAP[dst].begin(), MAP[src][i]);
        MAP[src].erase(MAP[src].begin() + i);
    }

    // box_to_pos 업데이트(src)
    for (int i = 0; i < MAP[src].size(); i++)
        box_to_pos[MAP[src][i]] = {src, i};
    // box_to_pos 업데이트(dst)
    for (int i = 0; i < MAP[dst].size(); i++)
        box_to_pos[MAP[dst][i]] = {dst, i};
    return MAP[dst].size();
}

int get_present(int num) {
    pair<int, int> p = box_to_pos[num];
    int a = (p.second - 1 < 0) ? -1 : MAP[p.first][p.second - 1];
    int b = (p.second + 1 >= MAP[p.first].size()) ? -1 : MAP[p.first][p.second + 1];
    return a + 2 * b;
}

int get_belt(int num) {
    vector<int> arr = MAP[--num];
    int c = arr.size();
    int a = (!c) ? -1 : arr[0];
    int b = (!c) ? -1 : arr[c - 1];
    return a + 2 * b + 3 * c;
}

int main() {
    // freopen("./input.txt", "r", stdin);
    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {
        int cmd, src, dst, num;
        cin >> cmd;
        if (200 <= cmd && cmd <= 400) {
            cin >> src >> dst;
            src--, dst--;
        } else if (cmd == 500 || cmd == 600)
            cin >> num;

        switch (cmd) {
        case 100:
            input();
            break;
        case 200:
            cout << move_to_all(src, dst) << endl;
            break;
        case 300:
            cout << change_to_front(src, dst) << endl;
            break;
        case 400:
            cout << divide_present(src, dst) << endl;
            break;
        case 500:
            cout << get_present(num) << endl;
            break;
        case 600:
            cout << get_belt(num) << endl;
            break;
        }
    }
    return 0;
}