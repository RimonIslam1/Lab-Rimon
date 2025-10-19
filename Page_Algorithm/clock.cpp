#include <bits/stdc++.h>
using namespace std;

vector<int> read_int_line(const string &prompt = "") {
    if (!prompt.empty()) cout << prompt;
    string line;
    // consume leftover newline if any
    getline(cin, line);
    while(line.size() == 0 && cin.good()) {
        // if empty line, try to read again (handles mixing >> and getline)
        getline(cin, line);
    }
    vector<int> vals;
    istringstream iss(line);
    int x;
    while (iss >> x) vals.push_back(x);
    return vals;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "Number of pages:\n";
    int num_pages;
    if (!(cin >> num_pages)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Reference string:\n";
    vector<int> ref_string = read_int_line();

    cout << "Number of frames:\n";
    int num_frames;
    cin >> num_frames;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Initialize frames with -1 meaning empty
    vector<int> memo(num_frames, -1);
    vector<int> reference(num_frames, 0);
    int pointer = 0; // clock pointer

    int total_page_fails = 0;

    for (int page : ref_string) {
        bool page_fault = false;

        // Check hit
        auto it = find(memo.begin(), memo.end(), page);
        if (it == memo.end()) {
            // page fault
            page_fault = true;
            total_page_fails++;

            // If empty frame exists
            auto empty_it = find(memo.begin(), memo.end(), -1);
            if (empty_it != memo.end()) {
                *empty_it = page;
                // set its reference bit to 1
                int idx = distance(memo.begin(), empty_it);
                reference[idx] = 1;
            } else {
                // Ask user to provide reference bits (to mirror original Python behavior)
                vector<int> newref;
                while (true) {
                    newref = read_int_line(string("Reference bits for inserting ") + to_string(page) + ":\n");
                    if ((int)newref.size() == num_frames) break;
                    cout << "Please enter exactly " << num_frames << " integers (0 or 1) separated by spaces.\n";
                }
                reference = newref;

                // Clock replacement: find a pointer with reference == 0, turning 1s to 0 as we go
                while (true) {
                    if (reference[pointer] == 0) {
                        memo[pointer] = page;
                        reference[pointer] = 1; // set to 1 on insertion as python did
                        pointer = (pointer + 1) % num_frames;
                        break;
                    } else {
                        reference[pointer] = 0;
                        pointer = (pointer + 1) % num_frames;
                    }
                }
            }
        } else {
            // hit: set its reference bit to 1
            int idx = distance(memo.begin(), it);
            reference[idx] = 1;
        }

        // Print per-step info similar to Python output
        cout << "\nProcessing page: " << page << "\n";
        cout << "Frames:\t\t  [ ";
        for (int v : memo) {
            if (v == -1) cout << "  " << " - ";
            else cout << v << " ";
        }
        cout << "]\n";

        cout << "Reference Bits:   [ ";
        for (int b : reference) cout << b << " ";
        cout << "]\n";

        cout << "Pointer at:\t   " << pointer << "\n";

        if (page_fault) cout << "Page Fault: PF|\n";
        else cout << "Page Fault:   |\n";

        cout << "----------------------------------------\n";
    }

    cout << "\nTotal page fails = " << total_page_fails << "\n";

    return 0;
}
