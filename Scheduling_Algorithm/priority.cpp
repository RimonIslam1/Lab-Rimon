#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;
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

void printResults(const vector<Process>& processes) {
    int n = processes.size();
    double totalWT = 0, totalTAT = 0;
    int lastCT = 0;
    cout << "\nPID\tAT\tBT\tPRI\tCT\tTAT\tWT\n";
    for (const auto& p : processes) {
        cout << "P" << p.pid << "\t" << p.arrivalTime << "\t" << p.burstTime << "\t"
             << p.priority << "\t" << p.completionTime << "\t" << p.turnaroundTime << "\t" << p.waitingTime << "\n";
        totalWT += p.waitingTime;
        totalTAT += p.turnaroundTime;
        lastCT = max(lastCT, p.completionTime);
    }
    cout << "\nAverage Waiting Time = " << totalWT / n;
    cout << "\nAverage Turnaround Time = " << totalTAT / n;
    cout << "\nThroughput = " << (double)n / lastCT << " process/unit time\n";
}

void printGanttChart(const vector<GanttBlock>& gantt) {
    cout << "\nGantt Chart:\n";
    for (const auto& block : gantt) {
        int width = block.end - block.start;
        cout << " ";
        for (int i = 0; i < width * 4; ++i) cout << "-";
    }
    cout << "\n";
    for (const auto& block : gantt) {
        int width = block.end - block.start;
        cout << "|";
        int left_space = (width * 4 - 2) / 2;
        int right_space = width * 4 - 2 - left_space;
        for (int i = 0; i < left_space; ++i) cout << " ";
        cout << "P" << block.pid;
        for (int i = 0; i < right_space; ++i) cout << " ";
    }
    cout << "|\n";
    for (const auto& block : gantt) {
        int width = block.end - block.start;
        cout << " ";
        for (int i = 0; i < width * 4; ++i) cout << "-";
    }
    cout << "\n";
    int last = gantt.front().start;
    cout << last;
    for (const auto& block : gantt) {
        int width = block.end - block.start;
        int pos = width * 4;
        cout << setw(pos) << block.end;
        last = block.end;
    }
    cout << "\n";
}

// Non-Preemptive Priority Scheduling
void Priority_NP(vector<Process> processes) {
    cout << "\n===== Priority Scheduling (Non-Preemptive) =====\n";
    int n = processes.size(), currentTime = 0, completed = 0;
    vector<bool> isCompleted(n, false);
    vector<GanttBlock> gantt;

    while (completed < n) {
        int idx = -1, highestPriority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!isCompleted[i] && processes[i].arrivalTime <= currentTime) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            currentTime++;
            continue;
        }
        gantt.push_back({processes[idx].pid, currentTime, currentTime + processes[idx].burstTime});
        currentTime += processes[idx].burstTime;
        processes[idx].completionTime = currentTime;
        processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
        processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
        isCompleted[idx] = true;
        completed++;
    }
    printGanttChart(gantt);
    printResults(processes);
}

// Preemptive Priority Scheduling
void Priority_P(vector<Process> processes) {
    cout << "\n===== Priority Scheduling (Preemptive) =====\n";
    int n = processes.size(), currentTime = 0, completed = 0;
    for (auto& p : processes) p.remainingTime = p.burstTime;
    vector<GanttBlock> gantt;

    int lastPid = -1;
    while (completed < n) {
        int idx = -1, highestPriority = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                if (processes[i].priority < highestPriority) {
                    highestPriority = processes[i].priority;
                    idx = i;
                }
            }
        }
        if (idx == -1) {
            currentTime++;
            continue;
        }
        if (lastPid != processes[idx].pid) {
            gantt.push_back({processes[idx].pid, currentTime, currentTime + 1});
        } else {
            gantt.back().end++;
        }
        processes[idx].remainingTime--;
        currentTime++;
        lastPid = processes[idx].pid;
        if (processes[idx].remainingTime == 0) {
            completed++;
            processes[idx].completionTime = currentTime;
            processes[idx].turnaroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
        }
    }
    printGanttChart(gantt);
    printResults(processes);
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        cout << "Enter Arrival Time, Burst Time, Priority for Process " << i + 1 << ": ";
        processes[i].pid = i + 1;
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
    }

    Priority_NP(processes);
    Priority_P(processes);

    return 0;
}