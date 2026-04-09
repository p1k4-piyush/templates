#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"
#include "../graphs/graph.hpp"
#include "../graphs/forest.hpp"
#include "../graphs/centroid.hpp"

int main() {
    forest<int> g(50);
    mt19937 rng(1337);
    for (int i = 1; i < 50; i++) {
        uniform_int_distribution<int> unif(0, i - 1);
        g.add(i, unif(rng));
    }
    
    auto res = centroid_decomposition(g);
    dbg("Centroid Decomposition Test", res);
    
    // Validate that there is a root in the centroid tree
    // meaning the root centroid node is validly populated
    assert(res.size() == 50);
    
    return 0;
}
