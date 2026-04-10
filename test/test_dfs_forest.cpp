#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/forest.hpp"
#include "../graphs/dfs_forest.hpp"

int main() {
    dfs_forest<int> g(15);
    mt19937 rng(1337);
    // Construct random forest/tree structure
    for (int i = 1; i < 15; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        g.add(unif(rng), i);
    }
    g.dfs_all();
    dbg("DFS Forest Test", g);
#ifdef GRACIE
    dbg("DFS Forest Subtree Size Test", graphviz(g, "sz"));
#endif
    return 0;
}
