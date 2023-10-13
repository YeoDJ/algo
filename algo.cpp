#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using namespace std;

/*
    <���� ��Ʈ>
    1. pid_to_idx
        ===> arr��� ������ ���ĵ� �ʿ䰡 �����Ƿ�
            pid�� ���� idx�� �ٷιٷ� ã�ư��� ���� map�� ���
    2. total_sum
        ===> startRace()���� ���õ��� ���� �䳢�� ������ �� ������ �Ѳ����� �ݿ��ϱ� ����
*/

struct rabbit {
    int pid, dist, y, x, jump;
    long long score;
};
// �̵��� �䳢 ������ ���� �Լ�
struct selectCmp {
    bool operator()(rabbit r1, rabbit r2) {
        if (r1.jump != r2.jump)
            return r1.jump > r2.jump;
        if (r1.y + r1.x != r2.y + r2.x)
            return r1.y + r1.x > r2.y + r2.x;
        if (r1.y != r2.y)
            return r1.y > r2.y;
        if (r1.x != r2.x)
            return r1.x > r2.x;
        return r1.pid > r2.pid;
    }
};

int dy[] = {-1, 1, 0, 0};
int dx[] = {0, 0, -1, 1};
int n, m, p;
// total_sum: �䳢�� ������ ������ '�� + ��' ���� ���⿡ ������
long long total_sum = 0;
vector<rabbit> arr;
// pid�� Ű�� �Ͽ� idx�� �ٷ� �����ϱ� ���� ����
unordered_map<int, int> pid_to_idx;

bool inRange(pair<int, int> p) { return 0 <= p.first && p.first < n && 0 <= p.second && p.second < m; }

// Race�� �ʿ��� ���� �Լ�
bool compare(rabbit r1, rabbit r2) {
    if (r1.y + r1.x != r2.y + r2.x)
        return r1.y + r1.x > r2.y + r2.x;
    if (r1.y != r2.y)
        return r1.y > r2.y;
    if (r1.x != r2.x)
        return r1.x > r2.x;
    return r1.pid > r2.pid;
}

void rsltPos(pair<int, int> &p, int py, int px) {
    // R(<->L): R, L, R / U(<->D): U, D, U
    pair<int, int> tmp_p = p;
    for (int i = 0; i < 3; i++) {
        if (inRange({p.first + py, p.second + px})) {
            p.first += py, p.second += px;
            break;
        }
        p.first = (py < 0) ? 0 : (py > 0) ? n - 1 : p.first;
        p.second = (px < 0) ? 0 : (px > 0) ? m - 1 : p.second;
        py = (py < 0) ? tmp_p.first - py : (py > 0) ? n - 1 - tmp_p.first - py : 0;
        px = (px < 0) ? tmp_p.second - px : (px > 0) ? m - 1 - tmp_p.second - px : 0;
        tmp_p = p;
    }
}

void input() {
    cin >> n >> m >> p;
    for (int i = 0; i < p; i++) {
        int pid, dist;
        cin >> pid >> dist;
        arr.push_back({pid, dist, 0, 0, 0, 0});
        pid_to_idx[pid] = i;
    }
}

void startRace() {
    int k, s;
    cin >> k >> s;
    priority_queue<rabbit, vector<rabbit>, selectCmp> pq;
    unordered_set<int> isPid;
    for (auto &&i : arr)
        pq.push(i);

    for (int t = 0; t < k; t++) {
        int rslt = 0;
        rabbit r = pq.top();
        pq.pop();
        vector<pair<int, int>> dist_rslt(4, {r.y, r.x});

        // �� ���⿡ ���� �̵��� ��ġ(���� ������ �ݴ� �������� �̵�)
        for (int i = 0; i < 4; i++)
            rsltPos(dist_rslt[i], (dy[i] * r.dist) % (2 * n - 2), (dx[i] * r.dist) % (2 * m - 2));
        // ��ġ �켱������ ���� �̵��� ��ġ ������ �� �̵�
        for (int i = 1; i < 4; i++)
            if (compare({0, 0, dist_rslt[i].first, dist_rslt[i].second, 0, 0}, {0, 0, dist_rslt[rslt].first, dist_rslt[rslt].second, 0, 0}))
                rslt = i;

        r.y = dist_rslt[rslt].first, r.x = dist_rslt[rslt].second, r.jump++;
        // r.score�� ���� ����: �̵��� �ڽ��� �� ������ ������ �ȵ�
        r.score -= r.y + r.x + 2;
        total_sum += r.y + r.x + 2;
        isPid.insert(r.pid);
        pq.push(r);
    }

    // ���ϱ�
    rabbit target = {0, 0, 0, 0, 0, 0};
    while (!pq.empty()) {
        rabbit r = pq.top();
        arr[pid_to_idx[r.pid]] = r;
        pq.pop();
        if (isPid.find(r.pid) != isPid.end() && compare(r, target))
            target = r;
    }
    arr[pid_to_idx[target.pid]].score += s;
}

int main() {
    freopen("./input.txt", "r", stdin);
    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {
        int cmd;
        cin >> cmd;
        switch (cmd) {
        case 100:
            input();
            break;
        case 200:
            startRace();
            break;
        case 300:
            int pid, l;
            cin >> pid >> l;
            arr[pid_to_idx[pid]].dist *= l;
            break;
        case 400:
            long long ans = -1;
            for (auto &&i : arr)
                ans = max(ans, i.score + total_sum);
            cout << ans;
            break;
        }
    }
    return 0;
}