#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

// 문제 주소, 문제 id, 채점기 id
// 대기 시작 시간: wt 초가 되었을 때 우선순위 따져서 채점 시작
// 채점 시작 시간, 채점 끝 시간, 우선 순위
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

// 현재 Jid를 사용할 수 있는가?
priority_queue<int, vector<int>, greater<int>> jid_arr;
// 300 명령어(채점 시작, (domain, pq<problem>))
unordered_map<string, priority_queue<problem, vector<problem>, compare>> waiting;
// (domain, stack<problem>)
unordered_map<string, stack<problem>> history;
// 300 명령어(도메인만 모아 둠)
unordered_set<string> j_domain, waiting_url;
// 400 명령어(채점 종료)
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

    // 채점 대기열 못넣는 경우
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

        // 절대 채점이 안되는 경우
        if (j_domain.find(p.domain) != j_domain.end())
            continue;
        if (history.find(p.domain) != history.end()) {
            int start = history[p.domain].top().st;
            int gap = history[p.domain].top().et - start;
            if (t < start + 3 * gap)
                continue;
        }
        // 우선순위 확인
        if (p.prior < min_prior || p.prior == min_prior && p.wt < min_wt) {
            min_prior = p.prior;
            min_wt = p.wt;
            domain = p.domain;
        }
    }

    // 채점 시작하기 전에 우선 순위, 대기 시간 초기화
    // 그리고 시작 시간 및 채점기 추가
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