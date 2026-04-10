#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/cycles.hpp"

int main() {
    undigraph<int> g(15);
    mt19937 rng(12453);
    for (int i = 1; i < 15; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        g.add(i, unif(rng));
    }
    for (int i = 0; i < 5; i++) {
        uniform_int_distribution<int> unif(0, 14);
        int u = unif(rng), v = unif(rng);
        if (u != v) g.add(u, v);
    }

    auto res = find_cycles(g);
#ifdef GRACIE
    dbg("Cycles Test", graphviz_cycles(g, res));
#else
    dbg("Cycles Test", res);
#endif
    return 0;
}
