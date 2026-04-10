#include "bits/stdc++.h"
using namespace std;
#include "../data/sparsetable.hpp"
#include "../template/dbg.hpp"

int main()
{
    vector<int> a(100);
    mt19937 rng(1337);
    for (int i = 0; i < 100; i++) {
        uniform_int_distribution<int> unif(1, 1000);
        a[i] = unif(rng);
    }
    SparseTable<int, std::function<int(int, int)>> st(a, [](int x, int y) { return min(x, y); });

    dbg("Sparse Table Scaling Test", st);
    assert(st.get(0, 99) > 0);
    return 0;
}
