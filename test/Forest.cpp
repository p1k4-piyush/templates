#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../graphs/graph.hpp"
#include "../graphs/forest.hpp"

int main()
{
    forest<int> g(6);
    g.add(0, 1, 1);
    g.add(1, 2, 2);
    g.add(3, 4, 1);
    g.add(4, 5, 3);
    dbg("Forest Test", g);
    return 0;
}
