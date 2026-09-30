#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

struct P {
    int id, at, bt, rt, ct = 0, tat = 0, wt = 0;
};

void result(vector<P> p) {
    float aw = 0, at = 0;

    cout << "\nP\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &x : p) {
        x.tat = x.ct - x.at;
        x.wt = x.tat - x.bt;

        cout << "P" << x.id << "\t" << x.at << "\t"
             << x.bt << "\t" << x.ct << "\t"
             << x.tat << "\t" << x.wt << endl;

        aw += x.wt;
        at += x.tat;
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage WT = " << aw / p.size();
    cout << "\nAverage TAT = " << at / p.size() << endl;
}

void gantt(vector<int> g, vector<int> t) {
    cout << "\nGantt Chart:\n";
    for (int x : g) cout << "| P" << x << " ";
    cout << "|\n";
    for (int x : t) cout << x << "\t";
    cout << endl;
}

// FCFS
void fcfs(vector<P> p) {
    sort(p.begin(), p.end(), [](P a, P b) {
        return a.at < b.at;
    });

    int time = 0;
    vector<int> g, t = {0};

    for (auto &x : p) {
        time = max(time, x.at);
        g.push_back(x.id);
        t.back() = time;
        time += x.bt;
        x.ct = time;
        t.push_back(time);
    }

    gantt(g, t);
    result(p);
}

// SJF Non-Preemptive
void sjf(vector<P> p) {
    int n = p.size(), time = 0, done = 0;
    vector<int> used(n, 0), g, t = {0};

    while (done < n) {
        int k = -1;

        for (int i = 0; i < n; i++)
            if (!used[i] && p[i].at <= time &&
                (k == -1 || p[i].bt < p[k].bt))
                k = i;

        if (k == -1) {
            time++;
            continue;
        }

        g.push_back(p[k].id);
        t.back() = time;

        time += p[k].bt;
        p[k].ct = time;

        t.push_back(time);
        used[k] = 1;
        done++;
    }

    gantt(g, t);
    result(p);
}

// Round Robin
void rr(vector<P> p, int q) {
    sort(p.begin(), p.end(), [](P a, P b) {
        return a.at < b.at;
    });

    int n = p.size(), time = 0, done = 0, next = 0;
    queue<int> Q;
    vector<int> g, t = {0};

    for (auto &x : p) x.rt = x.bt;

    while (done < n) {

        while (next < n && p[next].at <= time)
            Q.push(next++);

        if (Q.empty()) {
            time++;
            continue;
        }

        int i = Q.front();
        Q.pop();

        g.push_back(p[i].id);

        int run = min(q, p[i].rt);
        time += run;
        p[i].rt -= run;

        t.push_back(time);

        while (next < n && p[next].at <= time)
            Q.push(next++);

        if (p[i].rt > 0)
            Q.push(i);
        else {
            p[i].ct = time;
            done++;
        }
    }

    gantt(g, t);
    result(p);
}

// SRTF
void srtf(vector<P> p) {
    int n = p.size(), time = 0, done = 0, last = -1;
    vector<int> g, t = {0};

    for (auto &x : p) x.rt = x.bt;

    while (done < n) {
        int k = -1;

        for (int i = 0; i < n; i++)
            if (p[i].at <= time && p[i].rt > 0 &&
                (k == -1 || p[i].rt < p[k].rt))
                k = i;

        if (k == -1) {
            time++;
            continue;
        }

        if (k != last) {
            if (last != -1) t.push_back(time);
            g.push_back(p[k].id);
            last = k;
        }

        p[k].rt--;
        time++;

        if (p[k].rt == 0) {
            p[k].ct = time;
            done++;
        }
    }

    t.push_back(time);

    gantt(g, t);
    result(p);
}

// Multilevel Queue
// Queue 1 = Round Robin (High Priority)
// Queue 2 = FCFS (Low Priority)
void mlq(vector<P> p, int q) {
    vector<P> high, low;

    for (auto x : p) {
        int queue;
        cout << "P" << x.id << " Queue (1=High, 2=Low): ";
        cin >> queue;

        if (queue == 1) high.push_back(x);
        else low.push_back(x);
    }

    cout << "\n--- HIGH PRIORITY QUEUE (RR) ---\n";
    if (!high.empty()) rr(high, q);

    cout << "\n--- LOW PRIORITY QUEUE (FCFS) ---\n";
    if (!low.empty()) fcfs(low);
}

int main() {
    int n, choice, q;

    cout << "CPU SCHEDULING ALGORITHMS\n";
    cout << "Enter number of processes: ";
    cin >> n;

    vector<P> p(n);

    for (int i = 0; i < n; i++) {
        p[i].id = i + 1;

        cout << "\nP" << i + 1 << " Arrival Time: ";
        cin >> p[i].at;

        cout << "P" << i + 1 << " Burst Time: ";
        cin >> p[i].bt;
    }

    cout << "\n1. FCFS";
    cout << "\n2. Round Robin";
    cout << "\n3. SJF";
    cout << "\n4. SRTF";
    cout << "\n5. Multilevel Queue";

    cout << "\n\nEnter choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            fcfs(p);
            break;

        case 2:
            cout << "Enter Time Quantum: ";
            cin >> q;
            rr(p, q);
            break;

        case 3:
            sjf(p);
            break;

        case 4:
            srtf(p);
            break;

        case 5:
            cout << "Enter Time Quantum: ";
            cin >> q;
            mlq(p, q);
            break;

        default:
            cout << "Invalid Choice!";
    }

    return 0;
}