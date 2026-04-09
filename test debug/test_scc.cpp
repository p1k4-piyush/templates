#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/digraph.hpp"
#include "../graphs/scc.hpp"

int main() {
    digraph<int> g(100);
    mt19937 rng(888);
    for (int i = 0; i < 200; i++) {
        uniform_int_distribution<int> unif(0, 99);
        int u = unif(rng), v = unif(rng);
        if (u != v) g.add(u, v, 1);
    }
    int cnt;
    auto res = find_scc(g, cnt);
    dbg("SCC Scaling Test", res);
    assert(cnt <= 100);
    return 0;
}
