#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"

int main()
{
    undigraph<int> g(5);
    g.add(0, 1, 2);
    g.add(1, 2, 3);
    g.add(2, 3, 4);
    g.add(3, 4, 1);
    g.add(4, 0, 5);
    dbg("Undigraph Test", g);
    return 0;
}
