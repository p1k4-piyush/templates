#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/digraph.hpp"
#include "../graphs/graph.hpp"
#include "../graphs/scc.hpp"

int main()
{
    digraph<int> g(15);
    mt19937 rng(888);
    for (int i = 0; i < 20; i++) {
        uniform_int_distribution<int> unif(0, 14);
        int u = unif(rng), v = unif(rng);
        if (u != v)
            g.add(u, v, 1);
    }
    int cnt;
    auto res = find_scc(g, cnt);
#ifdef GRACIE
    dbg("SCC Scaling Test", graphviz_scc(g, res));
#else
    dbg("SCC Scaling Test", res);
#endif
    assert(cnt <= 15);
    return 0;
}
