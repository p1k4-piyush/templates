#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"
#include "../data/dsu.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/mst.hpp"

int main() {
    undigraph<int> g(15);
    mt19937 rng(999);
    for (int i = 1; i < 15; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        uniform_int_distribution<int> cost(1, 1000);
        g.add(i, unif(rng), cost(rng));
    }
    for (int i = 0; i < 5; i++) {
        uniform_int_distribution<int> unif(0, 14);
        uniform_int_distribution<int> cost(1, 1000);
        int u = unif(rng), v = unif(rng);
        if (u != v) g.add(u, v, cost(rng));
    }
    int ans = 0;
    auto res = find_mst(g, ans);
#ifdef GRACIE
    dbg("MST Scaling Test", graphviz_mst(g, res));
#else
    dbg("MST Scaling Test", res);
#endif
    assert(res.size() == 14);
    return 0;
}
