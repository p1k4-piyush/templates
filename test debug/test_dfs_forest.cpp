#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/forest.hpp"
#include "../graphs/dfs_forest.hpp"

int main() {
    dfs_forest<int> g(5);
    g.add(0, 1);
    g.add(0, 2);
    g.add(1, 3);
    g.add(1, 4);
    g.dfs_all();
    dbg("DFS Forest Test", g);
    return 0;
}
