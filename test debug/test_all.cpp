#include "bits/stdc++.h"
using namespace std;

#include "../template/dbg.hpp"
#include "../data/dsu.hpp"

// Required classes / structures
#include "../graphs/graph.hpp"
#include "../graphs/forest.hpp"
#include "../graphs/dfs_forest.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/digraph.hpp"
#include "../graphs/dfs_undigraph.hpp"

// Algorithms
#include "../graphs/bridges.hpp"
#include "../graphs/cutpoints.hpp"
#include "../graphs/cycles.hpp"
#include "../graphs/dijkstra.hpp"
#include "../graphs/eulerian.hpp"
#include "../graphs/mst.hpp"
#include "../graphs/scc.hpp"
#include "../graphs/topsort.hpp"

void test_scc() {
    digraph<int> g(4);
    g.add(0, 1, 1);
    g.add(1, 2, 1);
    g.add(2, 0, 1);
    g.add(2, 3, 1);
    int cnt;
    auto res = find_scc(g, cnt);
    dbg("SCC Test", res);
}

void test_dijkstra() {
    undigraph<int> g(4);
    g.add(0, 1, 5);
    g.add(0, 2, 2);
    g.add(2, 1, 1);
    g.add(1, 3, 4);
    auto res = dijkstra(g, 0);
    dbg("Dijkstra Test", res);
}

void test_mst() {
    undigraph<int> g(4);
    g.add(0, 1, 5);
    g.add(0, 2, 2);
    g.add(2, 1, 1);
    g.add(1, 3, 4);
    int ans = 0;
    auto res = find_mst(g, ans);
    dbg("MST test", res);
}

int main() {
    test_scc();
    test_dijkstra();
    test_mst();
    return 0;
}
