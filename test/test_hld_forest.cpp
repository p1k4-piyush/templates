#include "bits/stdc++.h"
using namespace std;
#include "../graphs/dfs_forest.hpp"
#include "../graphs/forest.hpp"
#include "../graphs/graph.hpp"
#include "../graphs/hld_forest.hpp"
#include "../template/dbg.hpp"

int main()
{
    int n = 100;
    hld_forest<int> g(n);
    mt19937 rng(12345);
    for (int i = 1; i < n; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        g.add(i, unif(rng));
    }

    g.build_hld_all();

    dbg("HLD Forest Scaling Test", g);

    // Assert tree has populated Heavy pointers
    assert((int)g.head.size() == n);

    return 0;
}
