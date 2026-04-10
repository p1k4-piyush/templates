#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/dfs_undigraph.hpp"

int main() {
    dfs_undigraph<int> g(15);
    mt19937 rng(42);
    // Construct spanning tree base
    for (int i = 1; i < 15; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        g.add(i, unif(rng));
    }
    // Inject cycles
    for (int i = 0; i < 5; i++) {
        uniform_int_distribution<int> unif(0, 14);
        int u = unif(rng), v = unif(rng);
        if (u != v) g.add(u, v);
    }
    g.dfs_all();
    dbg("DFS Undigraph Test", g);
#ifdef GRACIE
    dbg("DFS Undigraph Min Depth Test", graphviz(g, "min_depth"));
#endif
    return 0;
}
