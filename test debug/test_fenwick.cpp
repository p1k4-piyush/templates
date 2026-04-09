#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"
#include "../data/fenwick.hpp"

int main() {
    FenwickTree<long long> ft(100);
    mt19937 rng(42);
    for (int i = 0; i < 100; i++) {
        uniform_int_distribution<int> unif(1, 10);
        ft.Modify(i, unif(rng));
    }
    
    dbg("Fenwick Tree Scaling Test", ft);
    assert((int)ft.fenw.size() == 100);
    return 0;
}
