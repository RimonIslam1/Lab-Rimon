#include <bits/stdc++.h>
using namespace std;

void printFrames(const vector<int>& frames) {
    for (int f : frames) {
        if (f == -1) cout << "_ ";
        else cout << f << " ";
    }
    cout << endl;
}

// Utility function to print frames in tabular form
void printFramesTable(const vector<int>& frames) {
    cout << "|";
    for (int f : frames) {
        if (f == -1) cout << "   _   |";
        else cout << "  " << setw(3) << f << "  |";
    }
    cout << endl;
    cout << "+";
    for (size_t i = 0; i < frames.size(); ++i) cout << "-------+";
    cout << endl;
}

void printTableHeader(const vector<int>& seq) {
    cout << setw(10) << " ";
    for (int page : seq) {
        cout << setw(6) << page;
    }
    cout << endl;
    cout << setw(10) << " ";
    for (size_t i = 0; i < seq.size(); ++i) {
        cout << "------";
    }
    cout << endl;
}

void printTable(const vector<vector<int>>& frameStates, int framesCount, int n) {
    for (int f = 0; f < framesCount; ++f) {
        cout << setw(8) << "Frame " << f + 1 << " |";
        for (int t = 0; t < n; ++t) {
            if (frameStates[t][f] == -1)
                cout << setw(6) << "_";
            else
                cout << setw(6) << frameStates[t][f];
        }
        cout << endl;
    }
    cout << endl;
}

// Optimal Page Replacement
void optimal(int n, vector<int>& seq, int framesCount) {
    vector<int> frames(framesCount, -1);
    vector<vector<int>> frameStates(n, vector<int>(framesCount, -1));
    int faults = 0;
    cout << "\nOptimal Algorithm:\n";
    for (int i = 0; i < n; ++i) {
        int page = seq[i];
        if (find(frames.begin(), frames.end(), page) == frames.end()) {
            faults++;
            int idx = -1;
            for (int j = 0; j < framesCount; ++j) {
                if (frames[j] == -1) {
                    idx = j;
                    break;
                }
            }
            if (idx == -1) {
                int farthest = -1, pos = -1;
                for (int j = 0; j < framesCount; ++j) {
                    int next = find(seq.begin() + i + 1, seq.end(), frames[j]) - seq.begin();
                    if (next > farthest) {
                        farthest = next;
                        pos = j;
                    }
                }
                idx = pos;
            }
            frames[idx] = page;
        }
        frameStates[i] = frames;
    }
    printTableHeader(seq);
    printTable(frameStates, framesCount, n);
    cout << "Page Faults: " << faults << "\n\n";
}

// FIFO Page Replacement
void fifo(int n, vector<int>& seq, int framesCount) {
    vector<int> frames(framesCount, -1);
    vector<vector<int>> frameStates(n, vector<int>(framesCount, -1));
    queue<int> q;
    int faults = 0;
    cout << "\nFIFO Algorithm:\n";
    for (int i = 0; i < n; ++i) {
        int page = seq[i];
        if (find(frames.begin(), frames.end(), page) == frames.end()) {
            faults++;
            if (q.size() < framesCount) {
                for (int j = 0; j < framesCount; ++j) {
                    if (frames[j] == -1) {
                        frames[j] = page;
                        q.push(j);
                        break;
                    }
                }
            } else {
                int idx = q.front(); q.pop();
                frames[idx] = page;
                q.push(idx);
            }
        }
        frameStates[i] = frames;
    }
    printTableHeader(seq);
    printTable(frameStates, framesCount, n);
    cout << "Page Faults: " << faults << "\n\n";
}

// Second Chance Page Replacement
void secondChance(int n, vector<int>& seq, int framesCount, vector<int>& refBits) {
    vector<int> frames(framesCount, -1);
    vector<int> reference(framesCount, 0);
    vector<vector<int>> frameStates(n, vector<int>(framesCount, -1));
    int pointer = 0, faults = 0;
    cout << "\nSecond Chance Algorithm:\n";
    for (int i = 0; i < n; ++i) {
        int page = seq[i];
        auto it = find(frames.begin(), frames.end(), page);
        if (it == frames.end()) {
            faults++;
            while (reference[pointer] == 1) {
                reference[pointer] = 0;
                pointer = (pointer + 1) % framesCount;
            }
            frames[pointer] = page;
            reference[pointer] = 0;
            pointer = (pointer + 1) % framesCount;
        } else {
            int idx = it - frames.begin();
            reference[idx] = 1;
        }
        frameStates[i] = frames;
    }
    printTableHeader(seq);
    printTable(frameStates, framesCount, n);
    cout << "Page Faults: " << faults << "\n\n";
}

// NRU Page Replacement
void nru(int n, vector<int>& seq, int framesCount, vector<int>& refBits, vector<int>& modBits) {
    vector<int> frames(framesCount, -1);
    vector<int> reference(framesCount, 0);
    vector<int> modify(framesCount, 0);
    vector<vector<int>> frameStates(n, vector<int>(framesCount, -1));
    int faults = 0;
    for (int i = 0; i < n; ++i) {
        int page = seq[i];
        auto it = find(frames.begin(), frames.end(), page);
        if (it == frames.end()) {
            faults++;
            int classIdx = -1;
            if (i == 0) {
                reference = refBits;
                modify = modBits;
            }
            for (int c = 0; c < 4; ++c) {
                for (int j = 0; j < framesCount; ++j) {
                    if (frames[j] == -1) {
                        classIdx = j;
                        break;
                    }
                    if (c == 0 && reference[j] == 0 && modify[j] == 0) { classIdx = j; break; }
                    if (c == 1 && reference[j] == 0 && modify[j] == 1) { classIdx = j; break; }
                    if (c == 2 && reference[j] == 1 && modify[j] == 0) { classIdx = j; break; }
                    if (c == 3 && reference[j] == 1 && modify[j] == 1) { classIdx = j; break; }
                }
                if (classIdx != -1) break;
            }
            frames[classIdx] = page;
            reference[classIdx] = 1;
            modify[classIdx] = 0;
        } else {
            int idx = it - frames.begin();
            reference[idx] = 1;
        }
        frameStates[i] = frames;
    }
    cout << "\nNRU Algorithm:\n";
    printTableHeader(seq);
    printTable(frameStates, framesCount, n);
    cout << "Page Faults: " << faults << "\n\n";
}

int main() {
    int n, framesCount;
    cout << "Enter number of pages to be referenced: ";
    cin >> n;
    vector<int> seq(n);
    cout << "Enter page sequence: ";
    for (int i = 0; i < n; ++i) cin >> seq[i];
    cout << "Enter number of frames: ";
    cin >> framesCount;

    // For Second Chance
    vector<int> refBits(framesCount, 0);
    cout << "Enter reference bits for Second Chance (space separated, " << framesCount << " values): ";
    for (int i = 0; i < framesCount; ++i) cin >> refBits[i];

    // For NRU
    vector<int> nruRef(framesCount, 0), nruMod(framesCount, 0);
    cout << "Enter reference bits for NRU (space separated, " << framesCount << " values): ";
    for (int i = 0; i < framesCount; ++i) cin >> nruRef[i];
    cout << "Enter modification bits for NRU (space separated, " << framesCount << " values): ";
    for (int i = 0; i < framesCount; ++i) cin >> nruMod[i];

    optimal(n, seq, framesCount);
    fifo(n, seq, framesCount);
    secondChance(n, seq, framesCount, refBits);
    nru(n, seq, framesCount, nruRef, nruMod);

    return 0;
}