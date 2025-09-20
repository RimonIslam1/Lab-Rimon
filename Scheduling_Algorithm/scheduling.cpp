#include <bits/stdc++.h>
using namespace std;

struct Process {
    int pid;            
    int arrivalTime;    
    int burstTime;      
    int completionTime; 
    int waitingTime;    
    int turnaroundTime; 
    int responseTime;   
    int remainingTime;  
};

struct GanttBlock {
    int pid;
    int start;
    int end;
};

// Utility function to print results
void printResults(vector<Process> &processes) {
    int n = processes.size();
    double totalWaitingTime = 0, totalTurnaroundTime = 0;
    int lastCompletionTime = 0;

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &proc : processes) {
        cout << "P" << proc.pid << "\t" << proc.arrivalTime << "\t" << proc.burstTime << "\t"
             << proc.completionTime << "\t" << proc.turnaroundTime << "\t" << proc.waitingTime << "\n";
        totalWaitingTime += proc.waitingTime;
        totalTurnaroundTime += proc.turnaroundTime;
        lastCompletionTime = max(lastCompletionTime, proc.completionTime);
    }

    cout << "\nAverage Waiting Time = " << totalWaitingTime / n;
    cout << "\nAverage Turnaround Time = " << totalTurnaroundTime / n;
    cout << "\nThroughput = " << (double)n / lastCompletionTime << " process/unit time\n";
}

void printGanttChart(const vector<GanttBlock>& gantt) {
    cout << "\nGantt Chart:\n";

    // Print top border
    for (const auto& block : gantt) {
        int width = block.end - block.start;
        cout << " ";
        for (int i = 0; i < width * 4; ++i) cout << "-";
    }
    cout << "\n";

    // Print process blocks
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

    // Print bottom border
    for (const auto& block : gantt) {
        int width = block.end - block.start;
        cout << " ";
        for (int i = 0; i < width * 4; ++i) cout << "-";
    }
    cout << "\n";

    // Print time markers
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

// FCFS Scheduling
void FCFS(vector<Process> processes) {
    cout << "\n===== FCFS Scheduling =====\n";
    sort(processes.begin(), processes.end(), [](Process a, Process b) { return a.arrivalTime < b.arrivalTime; });

    int currentTime = 0;
    vector<GanttBlock> gantt;
    for (auto &proc : processes) {
        if (currentTime < proc.arrivalTime) currentTime = proc.arrivalTime;
        gantt.push_back({proc.pid, currentTime, currentTime + proc.burstTime});
        proc.completionTime = currentTime + proc.burstTime;
        proc.turnaroundTime = proc.completionTime - proc.arrivalTime;
        proc.waitingTime = proc.turnaroundTime - proc.burstTime;
        currentTime = proc.completionTime;
    }
    printGanttChart(gantt);
    printResults(processes);
}

// SJF Non-Preemptive Scheduling
void SJF_NP(vector<Process> processes) {
    cout << "\n===== SJF Non-Preemptive Scheduling =====\n";
    int n = processes.size(), currentTime = 0, completed = 0;
    vector<bool> isCompleted(n, false);
    vector<GanttBlock> gantt;

    while (completed < n) {
        int idx = -1, minBurstTime = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!isCompleted[i] && processes[i].arrivalTime <= currentTime && processes[i].burstTime < minBurstTime) {
                minBurstTime = processes[i].burstTime;
                idx = i;
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

// SJF Preemptive Scheduling
void SJF_P(vector<Process> processes) {
    cout << "\n===== SJF Preemptive Scheduling =====\n";
    int n = processes.size(), currentTime = 0, completed = 0;
    for (auto &proc : processes) proc.remainingTime = proc.burstTime;
    vector<GanttBlock> gantt;

    int lastPid = -1, startTime = 0;
    while (completed < n) {
        int idx = -1, minRemainingTime = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && processes[i].remainingTime < minRemainingTime) {
                minRemainingTime = processes[i].remainingTime;
                idx = i;
            }
        }
        if (idx == -1) {
            currentTime++;
            continue;
        }
        if (lastPid != processes[idx].pid) {
            if (lastPid != -1)
                gantt.back().end = currentTime;
            gantt.push_back({processes[idx].pid, currentTime, currentTime + 1});
            startTime = currentTime;
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
        // Extend the end time for the current block
        gantt.back().end = currentTime;
    }
    printGanttChart(gantt);
    printResults(processes);
}

// Round Robin Scheduling
void RoundRobin(vector<Process> processes, int quantum) {
    cout << "\n===== Round Robin Scheduling =====\n";
    int n = processes.size(), currentTime = 0, completed = 0;
    queue<int> processQueue;
    vector<bool> inQueue(n, false);
    vector<GanttBlock> gantt;

    for (auto &proc : processes) proc.remainingTime = proc.burstTime;

    sort(processes.begin(), processes.end(), [](Process a, Process b) { return a.arrivalTime < b.arrivalTime; });

    int idx = 0;
    while (completed < n) {
        while (idx < n && processes[idx].arrivalTime <= currentTime) {
            processQueue.push(idx);
            inQueue[idx] = true;
            idx++;
        }

        if (processQueue.empty()) {
            currentTime++;
            continue;
        }

        int i = processQueue.front();
        processQueue.pop();

        int execTime = min(quantum, processes[i].remainingTime);
        gantt.push_back({processes[i].pid, currentTime, currentTime + execTime});
        processes[i].remainingTime -= execTime;
        currentTime += execTime;

        while (idx < n && processes[idx].arrivalTime <= currentTime) {
            if (!inQueue[idx]) {
                processQueue.push(idx);
                inQueue[idx] = true;
            }
            idx++;
        }

        if (processes[i].remainingTime == 0) {
            completed++;
            processes[i].completionTime = currentTime;
            processes[i].turnaroundTime = processes[i].completionTime - processes[i].arrivalTime;
            processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
        } else {
            processQueue.push(i);
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
        cout << "Enter Arrival Time and Burst Time for Process " << i + 1 << ": ";
        processes[i].pid = i + 1;
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
    }

    FCFS(processes);
    SJF_NP(processes);
    SJF_P(processes);

    int quantum;
    cout << "Enter Time Quantum for Round Robin: ";
    cin >> quantum;
    RoundRobin(processes, quantum);

    return 0;
}
