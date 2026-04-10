#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/cutpoints.hpp"
#include "../graphs/dfs_undigraph.hpp"
#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"

int main()
{
    dfs_undigraph<int> g(15);
    mt19937 rng(123);
    for (int i = 1; i < 15; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        g.add(i, unif(rng));
    }
    for (int i = 0; i < 5; i++) {
        uniform_int_distribution<int> unif(0, 14);
        int u = unif(rng), v = unif(rng);
        if (u != v)
            g.add(u, v);
    }
    auto res = find_cutpoints(g);
#ifdef GRACIE
    dbg("Cutpoints Scaling Test", graphviz_cutpoints(g, res));
#else
    dbg("Cutpoints Scaling Test", res);
#endif
    assert(res.size() == 15);
    return 0;
}
