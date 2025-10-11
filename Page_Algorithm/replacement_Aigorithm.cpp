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

// Optimal Page Replacement
void optimal(int n, vector<int>& seq, int framesCount) {
    vector<int> frames(framesCount, -1);
    int faults = 0;
    cout << "\nOptimal Algorithm:\n";
    cout << "+";
    for (int i = 0; i < framesCount; ++i) cout << "-------+";
    cout << endl;
    for (int i = 0; i < n; ++i) {
        int page = seq[i];
        if (find(frames.begin(), frames.end(), page) != frames.end()) {
            printFramesTable(frames);
            continue;
        }
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
        printFramesTable(frames);
    }
    cout << "Page Faults: " << faults << "\n\n";
}

// FIFO Page Replacement
void fifo(int n, vector<int>& seq, int framesCount) {
    vector<int> frames(framesCount, -1);
    queue<int> q;
    int faults = 0;
    cout << "\nFIFO Algorithm:\n";
    cout << "+";
    for (int i = 0; i < framesCount; ++i) cout << "-------+";
    cout << endl;
    for (int i = 0; i < n; ++i) {
        int page = seq[i];
        if (find(frames.begin(), frames.end(), page) != frames.end()) {
            printFramesTable(frames);
            continue;
        }
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
        printFramesTable(frames);
    }
    cout << "Page Faults: " << faults << "\n\n";
}

// Second Chance Page Replacement
void secondChance(int n, vector<int>& seq, int framesCount, vector<int>& refBits) {
    vector<int> frames(framesCount, -1);
    vector<int> reference(framesCount, 0);
    int pointer = 0, faults = 0;
    cout << "\nSecond Chance Algorithm:\n";
    cout << "+";
    for (int i = 0; i < framesCount; ++i) cout << "-------+";
    cout << endl;
    for (int i = 0; i < n; ++i) {
        int page = seq[i];
        auto it = find(frames.begin(), frames.end(), page);
        if (it != frames.end()) {
            int idx = it - frames.begin();
            reference[idx] = 1;
            printFramesTable(frames);
            continue;
        }
        faults++;
        while (reference[pointer] == 1) {
            reference[pointer] = 0;
            pointer = (pointer + 1) % framesCount;
        }
        frames[pointer] = page;
        reference[pointer] = 0;
        pointer = (pointer + 1) % framesCount;
        printFramesTable(frames);
    }
    cout << "Page Faults: " << faults << "\n\n";
}

// NRU Page Replacement
void nru(int n, vector<int>& seq, int framesCount, vector<int>& refBits, vector<int>& modBits) {
    vector<int> frames(framesCount, -1);
    vector<int> reference(framesCount, 0);
    vector<int> modify(framesCount, 0);
    int faults = 0;
    cout << "\nNRU Algorithm:\n";
    cout << "+";
    for (int i = 0; i < framesCount; ++i) cout << "-------+";
    cout << endl;
    for (int i = 0; i < n; ++i) {
        int page = seq[i];
        auto it = find(frames.begin(), frames.end(), page);
        if (it != frames.end()) {
            int idx = it - frames.begin();
            reference[idx] = 1; // Set reference bit on hit
            printFramesTable(frames);
            continue;
        }
        faults++;
        int classIdx = -1;
        // On first page fault, initialize reference and modify bits from input
        if (i == 0) {
            reference = refBits;
            modify = modBits;
        }
        // Find victim according to NRU classes
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
        reference[classIdx] = 1; // Set reference bit on insertion
        modify[classIdx] = 0;    // Reset modify bit on insertion
        printFramesTable(frames);
    }
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