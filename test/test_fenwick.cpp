#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"
#include "../data/fenwick.hpp"

int main() {
    FenwickTree<long long> ft(16);
    mt19937 rng(42);
    for (int i = 0; i < 10; i++) {
        uniform_int_distribution<int> unif(1, 10);
        int x = unif(rng);
        cout << x << " ";
        ft.Modify(i, x);
    }
    cout << endl;
    
    dbg("Fenwick Tree Scaling Test", ft);
    assert((int)ft.fenw.size() == 16);
    return 0;
}
