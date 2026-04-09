#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/forest.hpp"
#include "../graphs/dfs_forest.hpp"
#include "../graphs/lca_forest.hpp"

int main() {
    lca_forest<int> g(15);
    mt19937 rng(7);
    for (int i = 1; i < 15; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        g.add(i, unif(rng));
    }
    
    g.dfs(0);
    g.build_lca();
    
    dbg("LCA Forest Scaling Test", g);
    assert(g.h > 0);
    return 0;
}
