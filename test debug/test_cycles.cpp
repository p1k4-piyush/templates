#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/cycles.hpp"

int main() {
    undigraph<int> g(4);
    g.add(0, 1);
    g.add(1, 2);
    g.add(2, 0); // Cycle 1
    g.add(2, 3); 
    g.add(3, 1); // Cycle 2
    auto res = find_cycles(g);
    dbg("Cycles Test", res);
    return 0;
}
