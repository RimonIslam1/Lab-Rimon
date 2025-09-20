#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int processId;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int waitingTime;
    int turnaroundTime;
    int remainingTime;
};

struct GanttBlock {
    int pid;
    int start;
    int end;
};

void printResults(const vector<Process>& p) {
    int n = p.size();
    double totalWT = 0, totalTAT = 0;
    int finishTime = 0;

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (const auto& pr : p) {
        cout << "P" << pr.processId << "\t" << pr.arrivalTime << "\t" << pr.burstTime << "\t"
             << pr.completionTime << "\t" << pr.turnaroundTime << "\t" << pr.waitingTime << "\n";
        totalWT += pr.waitingTime;
        totalTAT += pr.turnaroundTime;
        finishTime = max(finishTime, pr.completionTime);
    }

    cout << "\nAverage Waiting Time = " << totalWT / n;
    cout << "\nAverage Turnaround Time = " << totalTAT / n;
    cout << "\nThroughput = " << (double)n / finishTime << " process/unit time\n";
}

void printGanttChart(const std::vector<GanttBlock>& gantt) {
    cout << "\nGantt Chart:\n";

    // Top border
    cout << " ";
    for (const auto& block : gantt) {
        int width = block.end - block.start;
        for (int i = 0; i < width * 4; ++i) cout << "-";
        cout << " ";
    }
    cout << "\n";

    // Process blocks
    cout << "|";
    for (const auto& block : gantt) {
        int width = block.end - block.start;
        int left_space = (width * 4 - 2) / 2;
        int right_space = width * 4 - 2 - left_space;
        for (int i = 0; i < left_space; ++i) cout << " ";
        cout << "P" << block.pid;
        for (int i = 0; i < right_space; ++i) cout << " ";
        cout << "|";
    }
    cout << "\n";

    // Bottom border
    cout << " ";
    for (const auto& block : gantt) {
        int width = block.end - block.start;
        for (int i = 0; i < width * 4; ++i) cout << "-";
        cout << " ";
    }
    cout << "\n";

    // Time markers
    int last = gantt.front().start;
    cout << setw(2) << last;
    for (const auto& block : gantt) {
        int width = block.end - block.start;
        cout << setw(width * 4 + 1) << block.end;
        last = block.end;
    }
    cout << "\n";
}

void RoundRobin(vector<Process> p, int quantum) {
    int n = p.size(), time = 0, completed = 0;
    queue<int> q;
    vector<bool> inQueue(n, false);
    vector<GanttBlock> gantt;

    for (auto& pr : p) pr.remainingTime = pr.burstTime;

    // Sort by arrival time
    sort(p.begin(), p.end(), [](Process a, Process b) { return a.arrivalTime < b.arrivalTime; });

    int idx = 0;
    while (completed < n) {
        // Add processes that have arrived to the queue
        while (idx < n && p[idx].arrivalTime <= time) {
            q.push(idx);
            inQueue[idx] = true;
            idx++;
        }

        if (q.empty()) {
            time++;
            continue;
        }

        int i = q.front();
        q.pop();

        int execTime = min(quantum, p[i].remainingTime);
        gantt.push_back({p[i].processId, time, time + execTime});
        p[i].remainingTime -= execTime;
        time += execTime;

        // Add new arrivals during execution
        while (idx < n && p[idx].arrivalTime <= time) {
            if (!inQueue[idx]) {
                q.push(idx);
                inQueue[idx] = true;
            }
            idx++;
        }

        if (p[i].remainingTime == 0) {
            completed++;
            p[i].completionTime = time;
            p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
            p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;
        } else {
            q.push(i); // Put back if not finished
        }
    }
    printGanttChart(gantt);
    printResults(p);
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        cout << "Enter Arrival Time and Burst Time for Process " << i + 1 << ": ";
        p[i].processId = i + 1;
        cin >> p[i].arrivalTime >> p[i].burstTime;
    }

    int quantum;
    cout << "Enter Time Quantum for Round Robin: ";
    cin >> quantum;

    RoundRobin(p, quantum);

    return 0;
}