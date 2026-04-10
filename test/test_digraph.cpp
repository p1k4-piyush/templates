#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/digraph.hpp"
#include "../graphs/graph.hpp"

int main()
{
    digraph<int> g(4);
    g.add(0, 1, 1);
    g.add(1, 2, 2);
    g.add(2, 3, 3);
    g.add(3, 0, 4);
    dbg("Digraph Test", g);
    return 0;
}
