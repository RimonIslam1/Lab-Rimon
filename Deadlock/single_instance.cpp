#include <bits/stdc++.h>
using namespace std;

/*
 Single-instance deadlock detection (algorithm from prompt).
 Input format:
   n m
   m lines: u v   (directed arc u -> v)
 Nodes are 0-based (0 .. n-1). If your input is 1-based, subtract 1 when entering.
 The program tries each node as a start node; for each start it
 - marks all arcs unmarked
 - follows unmarked outgoing arcs, marking them as taken
 - records the path L and detects when a node repeats in L => cycle
 On finding a cycle the cycle is printed and the program exits.
*/

bool detectFrom(int start,
                const vector<vector<pair<int,int>>> &adj,
                int n, int m,
                vector<int> &outCycle) 
{
    // edge marks initially all false for this start
    vector<char> edgeMarked(m, 0);
    vector<int> L;          // path list
    int current = start;

    while (true) {
        // Step 3: add current to L and check if it appears twice
        L.push_back(current);

        // check for earlier occurrence of current in L (except last)
        int firstIdx = -1;
        for (int i = 0; i + 1 < (int)L.size(); ++i) {
            if (L[i] == current) { firstIdx = i; break; }
        }
        if (firstIdx != -1) {
            // cycle found: nodes from firstIdx..end of L
            outCycle.assign(L.begin() + firstIdx, L.end());
            return true;
        }

        // Step 4: find any unmarked outgoing arc from current
        int chosenEdge = -1;
        int chosenTo = -1;
        for (auto &p : adj[current]) {
            int to = p.first;
            int eid = p.second;
            if (!edgeMarked[eid]) { chosenEdge = eid; chosenTo = to; break; }
        }

        // Step 5: if found, mark it and follow
        if (chosenEdge != -1) {
            edgeMarked[chosenEdge] = 1;
            current = chosenTo;
            continue;
        }

        // Step 6: dead end
        if (current == start) {
            // reached start and no unmarked outgoing arcs => no cycle for this start
            return false;
        } else {
            // remove current and go back to previous
            L.pop_back();
            if (L.empty()) return false; // defensive
            current = L.back();
            continue;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<pair<int,int>> edges;
    vector<vector<pair<int,int>>> adj(n);
    edges.reserve(m);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        // assume input 0-based; if using 1-based, subtract 1 here:
        // u--; v--;
        edges.emplace_back(u, v);
        adj[u].push_back({v, i}); // store edge id i
    }

    // Try each node as start
    for (int s = 0; s < n; ++s) {
        vector<int> cycle;
        if (detectFrom(s, adj, n, m, cycle)) {
            cout << "Cycle found starting from node " << s << " :\n";
            for (size_t i = 0; i < cycle.size(); ++i) {
                if (i) cout << " -> ";
                cout << cycle[i];
            }
            cout << "\n";
            return 0;
        }
    }

    cout << "No cycles detected in the graph (no deadlock).\n";
    return 0;
}
