#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/digraph.hpp"
#include "../graphs/topsort.hpp"

int main() {
    digraph<int> g(5);
    g.add(0, 1);
    g.add(0, 2);
    g.add(1, 3);
    g.add(2, 3);
    g.add(3, 4);
    auto res = find_topsort(g);
#ifdef GRACIE
    dbg("Topsort Test", graphviz_topsort(g, res));
#else
    dbg("Topsort Test", res);
#endif
    return 0;
}
