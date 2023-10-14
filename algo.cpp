#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

// ���� �ּ�, ���� id, ä���� id
// ��� ���� �ð�: wt �ʰ� �Ǿ��� �� �켱���� ������ ä�� ����
// ä�� ���� �ð�, ä�� �� �ð�, �켱 ����
struct problem {
    string domain, pid;
    int id, wt, st, et, prior;
};
struct compare {
    bool operator()(problem p1, problem p2) {
        if (p1.prior != p2.prior)
            return p1.prior > p2.prior;
        return p1.wt > p2.wt;
    }
};

// ���� Jid�� ����� �� �ִ°�?
priority_queue<int, vector<int>, greater<int>> jid_arr;
// 300 ��ɾ�(ä�� ����, (domain, pq<problem>))
unordered_map<string, priority_queue<problem, vector<problem>, compare>> waiting;
// (domain, stack<problem>)
unordered_map<string, stack<problem>> history;
// 300 ��ɾ�(�����θ� ��� ��)
unordered_set<string> j_domain, waiting_url;
// 400 ��ɾ�(ä�� ����)
unordered_map<int, problem> judging;

void input() {
    int n;
    string url;
    cin >> n >> url;
    for (int i = 0; i < n; i++)
        jid_arr.push(i + 1);

    int pos = url.find('/');
    string domain = url.substr(0, pos);
    waiting[domain].push({domain, url.substr(pos + 1), 0, 0, 0, 0, 1});
    waiting_url.insert(url);
}

void request_ans() {
    int t, p;
    string u;
    cin >> t >> p >> u;

    // ä�� ��⿭ ���ִ� ���
    if (waiting_url.find(u) != waiting_url.end())
        return;
    int pos = u.find('/');
    string domain = u.substr(0, pos);
    waiting[domain].push({domain, u.substr(pos + 1), 0, t, 0, 0, p});
    waiting_url.insert(u);
}

void try_ans(int t, int id) {
    problem p;
    int min_prior = INT32_MAX;
    int min_wt = INT32_MAX;
    string domain = "";

    for (auto &&i : waiting) {
        p = i.second.top();

        // ���� ä���� �ȵǴ� ���
        if (j_domain.find(p.domain) != j_domain.end())
            continue;
        if (history.find(p.domain) != history.end()) {
            int start = history[p.domain].top().st;
            int gap = history[p.domain].top().et - start;
            if (t < start + 3 * gap)
                continue;
        }
        // �켱���� Ȯ��
        if (p.prior < min_prior || p.prior == min_prior && p.wt < min_wt) {
            min_prior = p.prior;
            min_wt = p.wt;
            domain = p.domain;
        }
    }

    // ä�� �����ϱ� ���� �켱 ����, ��� �ð� �ʱ�ȭ
    // �׸��� ���� �ð� �� ä���� �߰�
    if (min_prior != INT32_MAX) {
        jid_arr.pop();
        p = waiting[domain].top();
        waiting[domain].pop();
        waiting_url.erase(p.domain + '/' + p.pid);
        if (waiting[domain].empty())
            waiting.erase(domain);
        p.wt = 0, p.prior = 0, p.st = t, p.id = id;
        judging[p.id] = p, j_domain.insert(p.domain);
    }
}

void ans_end() {
    int t, id;
    cin >> t >> id;
    if (judging.find(id) == judging.end())
        return;

    problem p = judging[id];
    p.et = t, history[p.domain].push(p);
    judging.erase(id), j_domain.erase(p.domain);
    jid_arr.push(id);
}

int main() {
    freopen("./input.txt", "r", stdin);
    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {
        int cmd, t;
        cin >> cmd;
        if (cmd == 100)
            input();
        else if (cmd == 200)
            request_ans();
        else if (cmd == 300) {
            cin >> t;
            if (!jid_arr.empty() && !waiting.empty())
                try_ans(t, jid_arr.top());
        } else if (cmd == 400)
            ans_end();
        else if (cmd == 500) {
            cin >> t;
            cout << waiting_url.size() << endl;
        }
    }

    return 0;
}