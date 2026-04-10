#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/dijkstra.hpp"
#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"

int main()
{
    undigraph<int> g(10);
    mt19937 rng(444);
    for (int i = 1; i < 10; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        uniform_int_distribution<int> cost(1, 100);
        g.add(i, unif(rng), cost(rng));
    }
    for (int i = 0; i < 10; i++) {
        uniform_int_distribution<int> unif(0, 9);
        uniform_int_distribution<int> cost(1, 100);
        int u = unif(rng), v = unif(rng);
        if (u != v)
            g.add(u, v, cost(rng));
    }
    auto res = dijkstra(g, 0);
#ifdef GRACIE
    dbg("Dijkstra Scaling Test", graphviz_dijkstra(g, 0, res));
#else
    dbg("Dijkstra Scaling Test", res);
#endif
    assert(res[0] == 0);
    return 0;
}
