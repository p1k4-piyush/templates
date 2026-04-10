#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"
#include "../data/disjointsparsetable.hpp"

int main() {
    vector<int> a(100);
    mt19937 rng(1337);
    for (int i = 0; i < 100; i++) {
        uniform_int_distribution<int> unif(1, 999);
        a[i] = unif(rng);
    }
    DisjointSparseTable<int, std::function<int(int, int)>> dst(a, [](int x, int y) { return min(x, y); });
    
    dbg("Disjoint Sparse Table Scaling Test", dst);
    assert(dst.Query(0, 99) > 0);
    return 0;
}
