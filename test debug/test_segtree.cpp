#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../data/segment tree/layout.hpp"
#include "../data/segment tree/segment_tree.hpp"
#include "../data/segment tree/lazy.hpp"

struct Info {
    long long sum = 0;
    
    Info() {}
    Info(long long _sum) : sum(_sum) {}
    
    Info Unite(const Info& other) const {
        return Info(sum + other.sum);
    }
    
#ifdef GRACIE
    friend ostream& operator<<(ostream& os, const Info& info) {
        return os << info.sum;
    }
#endif
};

int main() {
    SimpleSegmentTree<Info> st(50);
    mt19937 rng(123);
    for (int i = 0; i < 50; i++) {
        uniform_int_distribution<int> unif(1, 100);
        st.Set(i, Info(unif(rng)));
    }
    
    dbg("Segment Tree Scaling Test", st);
    assert(st.Query(0, 49).sum > 0);
    return 0;
}
