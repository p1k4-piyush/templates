#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"
#include "../data/fenwick2d.hpp"

int main() {
    FenwickTree2D<int> ft(15, 15);
    mt19937 rng(42);
    for (int i = 0; i < 20; i++) {
        uniform_int_distribution<int> unif(0, 14);
        uniform_int_distribution<int> val(1, 100);
        ft.Modify(unif(rng), unif(rng), val(rng));
    }
    
    dbg("Fenwick 2D Scaling Test", ft);
    assert((int)ft.fenw.size() == 15);
    return 0;
}
