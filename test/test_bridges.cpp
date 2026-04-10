#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/dfs_undigraph.hpp"
#include "../graphs/bridges.hpp"

int main() {
    dfs_undigraph<int> g(15);
    mt19937 rng(1337);
    for (int i = 1; i < 15; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        g.add(i, unif(rng));
    }
    // Add extra random edges to create cycles (so not everything is a bridge)
    for (int i = 0; i < 7; i++) {
        uniform_int_distribution<int> unif(0, 14);
        int u = unif(rng), v = unif(rng);
        if (u != v) g.add(u, v);
    }
    auto res = find_bridges(g);
#ifdef GRACIE
    dbg("Bridges Scaling Test", graphviz_bridges(g, res));
#else
    dbg("Bridges Scaling Test", res);
#endif
    
    // Assert structural properties
    assert(res.size() == g.edges.size());
    return 0;
}
