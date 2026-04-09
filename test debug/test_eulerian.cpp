#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/eulerian.hpp"

int main() {
    undigraph<int> g(4);
    g.add(0, 1);
    g.add(1, 2);
    g.add(2, 3);
    g.add(3, 0);
    g.add(0, 2);
    int root;
    auto res = find_eulerian_path(g, root);
    dbg("Eulerian Test", res);
    return 0;
}
