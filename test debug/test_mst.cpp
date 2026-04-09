#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"
#include "../data/dsu.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/mst.hpp"

int main() {
    undigraph<int> g(100);
    mt19937 rng(999);
    for (int i = 1; i < 100; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        uniform_int_distribution<int> cost(1, 1000);
        g.add(i, unif(rng), cost(rng));
    }
    for (int i = 0; i < 150; i++) {
        uniform_int_distribution<int> unif(0, 99);
        uniform_int_distribution<int> cost(1, 1000);
        int u = unif(rng), v = unif(rng);
        if (u != v) g.add(u, v, cost(rng));
    }
    int ans = 0;
    auto res = find_mst(g, ans);
    dbg("MST Scaling Test", res);
    assert(res.size() == 99);
    return 0;
}
