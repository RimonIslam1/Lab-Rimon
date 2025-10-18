#include <bits/stdc++.h>
using namespace std;

int main() {
    //ios::sync_with_stdio(false);
    //cin.tie(nullptr);

    int P, R; // P = number of processes (v), R = number of resource types
    if (!(cin >> P >> R)) {
        cerr << "Failed to read number of processes and resources.\n";
        return 1;
    }

    vector<long long> available(R);
    for (int j = 0; j < R; ++j) {
        cin >> available[j];
    }

    vector<vector<long long>> allocation(P, vector<long long>(R));
    for (int i = 0; i < P; ++i)
        for (int j = 0; j < R; ++j)
            cin >> allocation[i][j];

    vector<vector<long long>> request(P, vector<long long>(R));
    for (int i = 0; i < P; ++i)
        for (int j = 0; j < R; ++j)
            cin >> request[i][j];

    // Set verbose = true to see iteration trace
    bool verbose = false;

    vector<bool> marked(P, false);
    bool changed = true;

    if (verbose) {
        cout << "Initial Available: ";
        for (auto x : available) cout << x << ' ';
        cout << "\n";
    }

    while (changed) {
        changed = false;
        for (int i = 0; i < P; ++i) {
            if (marked[i]) continue;

            bool can_finish = true;
            for (int j = 0; j < R; ++j) {
                if (request[i][j] > available[j]) {
                    can_finish = false;
                    break;
                }
            }

            if (can_finish) {
                // simulate process i finishing and releasing its allocated resources
                if (verbose) {
                    cout << "Process P" << i << " can finish. Releasing: ";
                    for (int j = 0; j < R; ++j) cout << allocation[i][j] << (j+1==R?'\n':' ');
                }
                for (int j = 0; j < R; ++j)
                    available[j] += allocation[i][j];
                marked[i] = true;
                changed = true;

                if (verbose) {
                    cout << "Available now: ";
                    for (auto x : available) cout << x << ' ';
                    cout << "\n";
                }
                // after marking, go back to searching from first process (optional)
            }
        }
    }

    // collect unmarked -> deadlocked
    vector<int> deadlocked;
    for (int i = 0; i < P; ++i)
        if (!marked[i]) deadlocked.push_back(i);

    if (deadlocked.empty()) {
        cout << "No deadlock; all processes can finish.\n";
    } else {
        cout << "Deadlocked processes (0-based indices): ";
        for (size_t k = 0; k < deadlocked.size(); ++k) {
            if (k) cout << ", ";
            cout << deadlocked[k];
        }
        cout << '\n';

        cout << "Deadlocked processes (1-based indices): ";
        for (size_t k = 0; k < deadlocked.size(); ++k) {
            if (k) cout << ", ";
            cout << (deadlocked[k] + 1);
        }
        cout << '\n';
    }

    return 0;
}
