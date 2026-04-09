#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/dfs_undigraph.hpp"
#include "../graphs/cutpoints.hpp"

int main() {
    dfs_undigraph<int> g(100);
    mt19937 rng(123);
    for (int i = 1; i < 100; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        g.add(i, unif(rng));
    }
    for (int i = 0; i < 15; i++) {
        uniform_int_distribution<int> unif(0, 99);
        int u = unif(rng), v = unif(rng);
        if (u != v) g.add(u, v);
    }
    auto res = find_cutpoints(g);
    dbg("Cutpoints Scaling Test", res);
    assert(res.size() == 100);
    return 0;
}
