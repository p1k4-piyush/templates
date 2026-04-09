#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/dfs_undigraph.hpp"

int main() {
    dfs_undigraph<int> g(5);
    g.add(0, 1);
    g.add(1, 2);
    g.add(2, 0); // cycle back-edge
    g.add(1, 3);
    g.add(3, 4);
    g.dfs_all();
    dbg("DFS Undigraph Test", g);
    return 0;
}
