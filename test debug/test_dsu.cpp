#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"
#include "../data/dsu.hpp"

int main() {
    dsu d(100);
    mt19937 rng(321);
    for (int i = 0; i < 60; i++) {
        uniform_int_distribution<int> unif(0, 99);
        d.unite(unif(rng), unif(rng));
    }
    
    dbg("DSU Scaling Test", d);
    assert((int)d.p.size() == 100);
    return 0;
}
