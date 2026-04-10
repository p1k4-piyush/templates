#include "bits/stdc++.h"
using namespace std;
#include "../graphs/digraph.hpp"
#include "../graphs/graph.hpp"
#include "../graphs/scc.hpp"
#include "../graphs/twosat.hpp"
#include "../template/dbg.hpp"

int main()
{
    int n = 8;
    twosat ts(n);
    mt19937 rng(54321);

    // Add logical conditions scaling to form interesting components across 8 elements
    for (int i = 0; i < 12; i++) {
        uniform_int_distribution<int> unif_node(0, n - 1);
        uniform_int_distribution<int> unif_bool(0, 1);
        ts.add(unif_node(rng), unif_bool(rng), unif_node(rng), unif_bool(rng));
    }

    dbg("TwoSat Scaling Test", ts);
    vector<int> res = ts.solve();
    // No assertion on !res.empty() since a random graphs might legitimately be unsatisfiable.

    return 0;
}
