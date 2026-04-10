#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/eulerian.hpp"

int main()
{
    undigraph<int> g(15);
    // Outer shell
    for (int i = 0; i < 15; i++) {
        g.add(i, (i + 1) % 15);
    }
    // Inner chord loop (guarantees even degree additions)
    for (int i = 0; i < 15; i++) {
        g.add(i, (i + 2) % 15);
    }
    // Cross bounds
    for (int i = 0; i < 15; i += 3) {
        g.add(i, (i + 3) % 15);
    }
    int root;
    auto res = find_eulerian_path(g, root);
#ifdef GRACIE
    dbg("Eulerian Test", graphviz_eulerian(g, res, root));
#else
    dbg("Eulerian Test", res);
#endif
    return 0;
}
