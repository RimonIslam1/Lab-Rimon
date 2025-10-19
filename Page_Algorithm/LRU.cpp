#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
using namespace std;

int main() {
    cout << "===== LRU (Least Recently Used) Page Replacement =====\n\n";

    int n;
    cout << "Enter number of page references: ";
    cin >> n;

    vector<int> pages(n);
    cout << "Enter page reference sequence: ";
    for (int i = 0; i < n; i++) cin >> pages[i];

    int framesCount;
    cout << "Enter number of frames: ";
    cin >> framesCount;

    vector<int> frames;
    unordered_map<int, int> recent;
    int pageFaults = 0, pageHits = 0;

    cout << "\n------------------------------------------------------\n";
    cout << setw(8) << "Page" << " | ";
    for (int i = 0; i < framesCount; i++)
        cout << "Frame" << i + 1 << " | ";
    cout << "Status\n";
    cout << "------------------------------------------------------\n";

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        bool found = false;

        // Check if the page is already in frames (HIT)
        for (auto &f : frames) {
            if (f == page) {
                found = true;
                pageHits++;
                recent[page] = i; // update usage time
                break;
            }
        }

        // If not found, it’s a FAULT
        if (!found) {
            pageFaults++;

            if (frames.size() == framesCount) {
                // Find LRU page (least recently used)
                int lruPage = frames[0];
                int minIndex = recent[lruPage];
                for (auto &f : frames) {
                    if (recent[f] < minIndex) {
                        minIndex = recent[f];
                        lruPage = f;
                    }
                }
                replace(frames.begin(), frames.end(), lruPage, page);
                recent.erase(lruPage);
            } else {
                frames.push_back(page);
            }

            recent[page] = i;
        }

        // Print the table row
        cout << setw(8) << page << " | ";
        for (int j = 0; j < framesCount; j++) {
            if (j < frames.size())
                cout << setw(5) << frames[j] << " | ";
            else
                cout << setw(5) << " " << " | ";
        }
        cout << (found ? "Hit" : "Fault") << "\n";
    }

    cout << "------------------------------------------------------\n";
    cout << "Total Page Faults = " << pageFaults << endl;
    cout << "Total Page Hits   = " << pageHits << endl;

    return 0;
}
//7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1