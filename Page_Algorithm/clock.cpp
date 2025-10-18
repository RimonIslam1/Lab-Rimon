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

// Clock (Second-Chance) page replacement
void clockAlgorithm(int n, const vector<int>& seq, int framesCount, vector<int> refBits) {
    if (framesCount <= 0) {
        cout << "Frames must be > 0\n";
        return;
    }

    vector<int> frames(framesCount, -1);
    vector<vector<int>> frameStates(n, vector<int>(framesCount, -1));
    vector<string> hitFault(n, "");
    int pointer = 0;
    int faults = 0;

    // Ensure refBits has correct size
    if ((int)refBits.size() != framesCount) refBits.assign(framesCount, 0);

    for (int i = 0; i < n; ++i) {
        int page = seq[i];
        auto it = find(frames.begin(), frames.end(), page);
        if (it != frames.end()) {
            // Hit
            int idx = int(it - frames.begin());
            refBits[idx] = 1;            // set reference bit on hit
            hitFault[i] = "PH";
        } else {
            // Fault -> find victim using clock algorithm
            faults++;
            hitFault[i] = "PF";
            while (true) {
                // empty frame => use immediately
                if (frames[pointer] == -1) {
                    frames[pointer] = page;
                    refBits[pointer] = 1;
                    pointer = (pointer + 1) % framesCount;
                    break;
                }
                if (refBits[pointer] == 0) {
                    // replace this frame
                    frames[pointer] = page;
                    refBits[pointer] = 1; // set ref bit after insertion
                    pointer = (pointer + 1) % framesCount;
                    break;
                }
                // give second chance
                refBits[pointer] = 0;
                pointer = (pointer + 1) % framesCount;
            }
        }
        frameStates[i] = frames;
    }

    cout << "\nClock (Second-Chance) Algorithm:\n";
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

    cout << "Do you want to provide initial reference bits? (y/n): ";
    char ch; cin >> ch;
    vector<int> refBits(framesCount, 0);
    if (ch == 'y' || ch == 'Y') {
        cout << "Enter " << framesCount << " reference bits (0 or 1): ";
        for (int i = 0; i < framesCount; ++i) cin >> refBits[i];
    }

    clockAlgorithm(n, seq, framesCount, refBits);
    return 0;
}