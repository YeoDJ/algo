#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;

int n, m;
vector<list<int>> MAP;
// 해당 box 번호가 어느 벨트에 있는지 저장
unordered_map<int, pair<int, int>> box_to_belt;

void input() {
    cin >> n >> m;
    MAP = vector<list<int>>(n, list<int>());
    for (int i = 0; i < m; i++) {
        int num;
        cin >> num;
        MAP[num - 1].push_back(i + 1);
        box_to_belt[i + 1] = {num - 1, MAP[num - 1].size() - 1};
    }
}

int move_to_all(int src, int dst) {
    int sz = MAP[src].size();
    if (!sz)
        return MAP[dst].size();

    // 원래 dst에 있는 선물 상자의 box_to_belt 업데이트
    for (auto &&i : MAP[dst])
        box_to_belt[i].second += sz;
    // 선물 상자 옮기기
    for (int i = sz - 1; i >= 0; i--) {
        int val = MAP[src].back();
        // box_to_belt 업데이트
        box_to_belt[val] = {dst, i};
        MAP[dst].push_front(val);
        MAP[src].pop_back();
    }
    return MAP[dst].size();
}

int change_to_front(int src, int dst) {
    int ssz = MAP[src].size(), dsz = MAP[dst].size(), val, val2;
    if (!ssz && !dsz)
        return dsz;

    // box_to_belt 업데이트 후 선물 상자 옮기기
    if (!ssz) {
        for (auto &&i : MAP[dst])
            box_to_belt[i].second--;
        val = MAP[dst].front();
        MAP[src].push_front(val);
        MAP[dst].pop_front();
        box_to_belt[val] = {src, 0};
        dsz--;
    } else if (!dsz) {
        for (auto &&i : MAP[src])
            box_to_belt[i].second--;
        val = MAP[src].front();
        MAP[dst].push_front(val);
        MAP[src].pop_front();
        box_to_belt[val] = {dst, 0};
        dsz++;
    } else {
        val2 = MAP[src].front();
        val = MAP[dst].front();
        MAP[src].pop_front();
        MAP[dst].pop_front();
        MAP[src].push_front(val);
        MAP[dst].push_front(val2);
        box_to_belt[val] = {src, 0};
        box_to_belt[val2] = {dst, 0};
    }
    return dsz;
}

int divide_present(int src, int dst) {
    int sz = MAP[src].size() / 2, val;
    if (!sz)
        return MAP[dst].size();

    // 원래 dst에 있는 선물 상자의 box_to_belt 업데이트
    for (auto &&i : MAP[dst])
        box_to_belt[i].second += sz;
    // 선물 상자 옮기기
    list<int> tmp;
    for (int i = 0; i < sz; i++) {
        tmp.push_back(MAP[src].front());
        MAP[src].pop_front();
    }
    for (int i = sz - 1; i >= 0; i--) {
        val = tmp.back();
        MAP[dst].push_front(val);
        box_to_belt[val] = {dst, i};
        tmp.pop_back();
    }
    for (auto &&i : MAP[src])
        box_to_belt[i].second -= sz;
    return MAP[dst].size();
}

int find_box(pair<int, int> p) {
    for (auto &&i : box_to_belt)
        if (i.second == p)
            return i.first;
    return -1;
}

int get_present(int num) {
    pair<int, int> p = box_to_belt[num];
    int a = (p.second - 1 < 0) ? -1 : find_box({p.first, p.second - 1});
    int b = (p.second + 1 >= MAP[p.first].size()) ? -1 : find_box({p.first, p.second + 1});
    return a + 2 * b;
}

int get_belt(int num) {
    num--;
    int c = MAP[num].size();
    int a = (!c) ? -1 : MAP[num].front();
    int b = (!c) ? -1 : MAP[num].back();
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