#include <bits/stdc++.h>
using namespace std;

void printTableHeader(const vector<int>& seq) {
    cout << setw(10) << " ";
    for (int page : seq) cout << setw(6) << page;
    cout << "\n" << setw(10) << " ";
    for (size_t i = 0; i < seq.size(); ++i) cout << "------";
    cout << "\n";
}

void printTable(const vector<vector<int>>& frameStates, const vector<string>& hitFault, int framesCount, int n) {
    for (int f = 0; f < framesCount; ++f) {
        cout << setw(8) << "Frame " << f + 1 << " |";
        for (int t = 0; t < n; ++t) {
            int v = frameStates[t][f];
            if (v == -1) cout << setw(6) << "_";
            else cout << setw(6) << v;
        }
        cout << "\n";
    }
    cout << setw(10) << " ";
    for (int t = 0; t < n; ++t) cout << setw(6) << hitFault[t];
    cout << "\n\n";
}

void lruAlgorithm(int n, const vector<int>& seq, int framesCount) {
    if (framesCount <= 0) {
        cout << "Frames must be > 0\n";
        return;
    }

    vector<int> frames(framesCount, -1);
    vector<int> lastUsed(framesCount, -1); // timestamp of last use
    vector<vector<int>> frameStates(n, vector<int>(framesCount, -1));
    vector<string> hitFault(n, "");
    int faults = 0;
    int time = 0;

    for (int i = 0; i < n; ++i) {
        int page = seq[i];
        auto it = find(frames.begin(), frames.end(), page);
        if (it != frames.end()) {
            // Hit
            int idx = int(it - frames.begin());
            lastUsed[idx] = time++;
            hitFault[i] = "PH";
        } else {
            // Fault
            faults++;
            hitFault[i] = "PF";
            // find empty frame
            int emptyIdx = -1;
            for (int j = 0; j < framesCount; ++j) {
                if (frames[j] == -1) { emptyIdx = j; break; }
            }
            if (emptyIdx != -1) {
                frames[emptyIdx] = page;
                lastUsed[emptyIdx] = time++;
            } else {
                // find least recently used
                int lruIdx = 0;
                int minTime = lastUsed[0];
                for (int j = 1; j < framesCount; ++j) {
                    if (lastUsed[j] < minTime) {
                        minTime = lastUsed[j];
                        lruIdx = j;
                    }
                }
                frames[lruIdx] = page;
                lastUsed[lruIdx] = time++;
            }
        }
        frameStates[i] = frames;
    }

    cout << "\nLRU Page Replacement Algorithm:\n";
    printTableHeader(seq);
    printTable(frameStates, hitFault, framesCount, n);
    cout << "Page Faults: " << faults << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, framesCount;
    cout << "Enter number of pages to be referenced: ";
    if (!(cin >> n) || n <= 0) return 0;

    vector<int> seq(n);
    cout << "Enter page sequence (space separated): ";
    for (int i = 0; i < n; ++i) cin >> seq[i];

    cout << "Enter number of frames: ";
    cin >> framesCount;
    if (framesCount <= 0) return 0;

    lruAlgorithm(n, seq, framesCount);
    return 0;
}