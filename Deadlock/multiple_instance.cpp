/*
4 3

4 2 3 1
2 1 0 0

0 0 1 0
2 0 0 1
0 1 2 0

2 0 0 1
1 0 1 0
2 1 0 0
*/

#include <bits/stdc++.h>

using namespace std;

int row_less(vector <int> &u, vector <int> &v)
{
    int i;

    for (i = 0; i < u.size(); i++) {
        if (u[i] > v[i]) {
            return false;
        }
    }

    return true;
}

int32_t main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int processes, resources;
    int i, j, change;

    // num_res, num_proc
    cin >> resources >> processes;

    vector <int> E(resources, 0), A(resources, 0);
    vector <vector <int> > C(processes, vector <int> (resources, 0)), R(processes, vector <int> (resources, 0));
    vector <bool> marked(processes, false);

    // existing
    for (i = 0; i < resources; i++) {
        cin >> E[i];
    }

    // available
    for (i = 0; i < resources; i++) {
        cin >> A[i];
    }

    // current allocation
    for (i = 0; i < processes; i++) {
        for (j = 0; j < resources; j++) {
            cin >> C[i][j];
        }
    }

    // request
    for (i = 0; i < processes; i++) {
        for (j = 0; j < resources; j++) {
            cin >> R[i][j];
        }
    }

    while (count(marked.begin(), marked.end(), false)) {
        change = 0;

        for (i = 0; i < processes; i++) {
            if (!marked[i]) {
                if (row_less(R[i], A)) {
                    for (j = 0; j < resources; j++) {
                        A[j] += C[i][j];

                        C[i][j] = R[i][j] = 0;
                    }

                    marked[i] = true;

                    change = 1;
                }
            }
        }

        if (!change) {
            break;
        }
    }

    if (count(marked.begin(), marked.end(), false)) {
        cout << "Deadlock detected\n\n";

        cout << "A:\n";

        for (i = 0; i < resources; i++) {
            cout << A[i] << " ";
        }

        cout << "\n\n";

        cout << "C:\n";

        for (i = 0; i < processes; i++) {
            for (j = 0; j < resources; j++) {
                cout << C[i][j] << " ";
            }

            cout << "\n";
        }

        cout << "\n";

        cout << "R:\n";

        for (i = 0; i < processes; i++) {
            for (j = 0; j < resources; j++) {
                cout << R[i][j] << " ";
            }

            cout << "\n";
        }

        cout << "\n";
    }
    else {
        cout << "No Deadlock detected\n";
    }

    return 0;
}