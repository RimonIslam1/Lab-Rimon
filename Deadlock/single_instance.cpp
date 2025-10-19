
/*
13 13
R A
A S
C S
F S
W F
D S
U D
G U
D T
B T
T E
E V
V G
*/

#include <bits/stdc++.h>

using namespace std;

struct edge {
    char node;
    bool marked;

    edge() : node(0), marked(false) {}

    edge(char node) : node(node), marked(false) {}
};

void dfs(char u, map <char, vector <edge> > &adj_list, vector <char> &path, int &f)
{
    if (!f) {
        return;
    }

    for (auto &v : adj_list[u]) {
        if (!v.marked) {
            v.marked = true;

            if (find(path.begin(), path.end(), v.node) == path.end()) {
                path.push_back(v.node);

                dfs(v.node, adj_list, path, f);

                if (!f) {
                    return;
                }

                path.pop_back();
            }
            else {
                f = 0;

                return;
            }
        }
    }
}

int32_t main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, i, f = 1;
    char u, v;
    map <char, vector <edge> > adj_list;
    map <char, bool> visited;
    vector <char> path;

    // num_nodes, num_edges
    cin >> n >> m;

    for (i = 0; i < m; i++) {
        // u -> v
        cin >> u >> v;

        adj_list[u].push_back(v);

        visited[u] = visited[v] = false;
    }

    for (auto &it : adj_list) {
        path.clear();

        dfs(it.first, adj_list, path, f);

        if (!f) {
            cout << "Deadlock detected.\nCycle path:\n";

            for (auto &it : path) {
                cout << it << " ";
            }

            cout << "\n";

            break;
        }
    }

    if (f) {
        cout << "No Deadlock detected\n";
    }

    return 0;
}
