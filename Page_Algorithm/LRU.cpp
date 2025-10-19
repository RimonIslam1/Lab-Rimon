#include <bits/stdc++.h>
using namespace std;

vector<int> parse_line_of_ints() {
    string line;
    getline(cin, line);
    while(line.size() == 0 && cin.good()) getline(cin, line);
    istringstream iss(line);
    vector<int> vals;
    int x;
    while (iss >> x) vals.push_back(x);
    return vals;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Input (keeps the same order as the Python program)
    int num_pages;
    if (!(cin >> num_pages)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<int> ref_string = parse_line_of_ints();

    int num_frames;
    cin >> num_frames;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // LRU structures
    vector<int> memo;                // frames content (order not meaningful)
    vector<int> recent_usage;        // LRU list: front = least recent, back = most recent

    vector<vector<string>> table;    // each element is a row (to be transposed later)
    int total_page_fails = 0;

    for (int page : ref_string) {
        int pf = 0;

        auto it_in_memo = find(memo.begin(), memo.end(), page);
        if (it_in_memo != memo.end()) {
            // Hit: move page to the back of recent_usage (most recent)
            auto it_ru = find(recent_usage.begin(), recent_usage.end(), page);
            if (it_ru != recent_usage.end()) recent_usage.erase(it_ru);
            recent_usage.push_back(page);
        } else {
            // Miss
            if ((int)memo.size() < num_frames) {
                memo.push_back(page);
                recent_usage.push_back(page);
            } else {
                // Evict LRU (front of recent_usage)
                int lru_page = recent_usage.front();
                recent_usage.erase(recent_usage.begin());
                // find index in memo and replace
                auto idx = find(memo.begin(), memo.end(), lru_page) - memo.begin();
                if (idx >= 0 && idx < (int)memo.size()) {
                    memo[idx] = page;
                } else {
                    // should not happen, but fallback: replace first frame
                    memo[0] = page;
                }
                recent_usage.push_back(page);
            }
            pf = 1;
            total_page_fails++;
        }

        string pf_str = pf ? "PF|" : "  |";

        // Build row to match Python: [page+" |"] + ["- |"] + memo entries + "-" fillers + ["- |"] + [pf]
        vector<string> row;
        row.push_back(to_string(page) + " |");
        row.push_back(string("- |"));
        // memo entries (in current order)
        for (int v : memo) row.push_back(to_string(v) + " |");
        // fill remaining empty frames with "- |"
        for (int i = 0; i < num_frames - (int)memo.size(); ++i) row.push_back(string("- |"));
        row.push_back(string("- |"));
        row.push_back(pf_str);

        table.push_back(row);
    }

    // Insert header rows (matching Python)
    // first header: a row of '|' repeated (num_frames + 4) entries
    vector<string> border_row(num_frames + 4, "|");
    table.insert(table.begin(), border_row);

    // second header: ["Frame/Page"] + ["-"*10] + ["1", "2", ... upto num_frames] padded to 10 chars + ["-"*10] + ["Page Fail "]
    vector<string> header;
    header.push_back("Frame/Page");
    header.push_back(string(10, '-'));
    for (int i = 1; i <= num_frames; ++i) {
        string s = to_string(i);
        // pad to width 10 (right padded with spaces)
        if ((int)s.size() < 10) s += string(10 - (int)s.size(), ' ');
        header.push_back(s);
    }
    header.push_back(string(10, '-'));
    header.push_back("Page Fail ");
    table.insert(table.begin(), header);

    // Transpose table (convert list of rows into list of columns for printing similarly to Python)
    // Determine sizes
    if (table.empty()) return 0;
    size_t rows = table.size();           // number of rows (will become columns after transpose)
    size_t cols = table[0].size();        // number of columns per row (should be num_frames + 4)

    // Compute a separator length roughly equal to the length of the first printed line
    string first_line_concat;
    for (size_t c = 0; c < rows; ++c) {
        first_line_concat += table[c][0];
    }
    size_t sep_len = first_line_concat.size() + rows;

    cout << string(sep_len, '-') << "\n\n";

    // print transpose: for each column index (0..cols-1), print table[row][col] for row=0..rows-1
    for (size_t c = 0; c < cols; ++c) {
        for (size_t r = 0; r < rows; ++r) {
            cout << table[r][c];
        }
        cout << "\n";
    }

    cout << "\n" << string(sep_len, '-') << "\n\n";
    cout << "Total page fails = " << total_page_fails << "\n";

    return 0;
}
