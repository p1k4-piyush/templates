#include <bits/stdc++.h>
using namespace std;

#include "../template/dbg.hpp"
#include "../data/dsu.hpp"
#include "../data/fenwick.hpp"
#include "../data/fenwick2d.hpp"
#include "../data/sparsetable.hpp"
#include "../data/disjointsparsetable.hpp"
#include "../data/segment tree/layout.hpp"
#include "../data/segment tree/segment_tree.hpp"
#include "../data/segment tree/lazy.hpp"

// Info requires strict definition
struct MyInfo {
    int v = 0;
    MyInfo Unite(const MyInfo& o) const { return {v + o.v}; }
    friend ostream& operator<<(ostream& os, const MyInfo& i) { return os << i.v; }
};

struct SafeInfo {
    vector<int> a;
    SafeInfo Unite(const SafeInfo& o) const { return SafeInfo{a}; }
};

struct SafeTag {
    int add = 0;
    bool ApplyTo(SafeInfo& info, int l, int r) const { return true; }
    void ApplyTo(SafeTag& tag) const { tag.add += add; }
    bool Empty() const { return add == 0; }
};

void test_ds() {
    dsu d(5);
    d.unite(0, 1);
    d.unite(1, 2);
    d.unite(3, 4);
    dbg("DSU Connectivity Check", d);

    FenwickTree<long long> fw(15);
    for (int i = 0; i < 15; i++) fw.Modify(i, i + 1);
    dbg("Fenwick Tree Sums", fw);
    
    FenwickTree2D<int> fw2(3, 3);
    fw2.Modify(0, 0, 5);
    fw2.Modify(1, 1, 2);
    dbg("Fenwick 2D Log", fw2);

    auto func = [](int a, int b) { return min(a, b); };
    vector<int> a = {5, 2, 8, 3, 9, 1};
    SparseTable<int, decltype(func)> st(a, func);
    dbg("Sparse Table RMSQ", st);
    
    DisjointSparseTable<int, decltype(func)> dst(a, func);
    dbg("Disjoint Sparse Table RMSQ", dst);

    vector<MyInfo> ai = {{5}, {2}, {8}, {3}};
    SimpleSegmentTree<MyInfo> sg(ai);
    dbg("Segment Tree Array Layout", sg);
    
    vector<SafeInfo> sl = {{{1,2}}, {{3}}, {{4,5,6}}, {{}}};
    LazySegmentTree<SafeInfo, SafeTag> lsg(sl);
    lsg.Modify(0, 2, {10});
    dbg("Lazy Segment Tree Safe Arrays", lsg);
}

int main() {
    test_ds();
    return 0;
}
