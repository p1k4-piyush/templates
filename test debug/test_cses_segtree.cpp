#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"

#include "../data/segment tree/layout.hpp"
#include "../data/segment tree/segment_tree.hpp"
#include "../data/segment tree/lazy.hpp"

// CSES Subarray Sum Queries
struct SubarraySumInfo {
    long long sum, pref, suff, ans;
    
    SubarraySumInfo() {
        sum = pref = suff = ans = 0;
    }
    
    SubarraySumInfo(long long val) {
        sum = val;
        pref = suff = ans = max(0LL, val); // empty subarray allowed
    }
    
    SubarraySumInfo Unite(const SubarraySumInfo& other) const {
        SubarraySumInfo res;
        res.sum = sum + other.sum;
        res.pref = max(pref, sum + other.pref);
        res.suff = max(other.suff, suff + other.sum);
        res.ans = max({ans, other.ans, suff + other.pref});
        return res;
    }

#ifdef GRACIE
    std::string print() const {
        std::ostringstream os;
        os << "[" << sum << "|" << pref << "|" << suff << "|" << ans << "]";
        return os.str();
    }
#endif
};

// CSES Range Updates and Sums + Range Mins
struct RangeUpdateInfo {
    long long sum, min_val, max_val;
    
    RangeUpdateInfo() : sum(0), min_val(1e18), max_val(-1e18) {}
    RangeUpdateInfo(long long val) : sum(val), min_val(val), max_val(val) {}
    
    RangeUpdateInfo Unite(const RangeUpdateInfo& other) const {
        RangeUpdateInfo res;
        res.sum = sum + other.sum;
        res.min_val = min(min_val, other.min_val);
        res.max_val = max(max_val, other.max_val);
        return res;
    }

#ifdef GRACIE
    std::string print() const {
        std::ostringstream os;
        os << "(s:" << sum << " m:" << min_val << " M:" << max_val << ")";
        return os.str();
    }
#endif
};

struct RangeUpdateTag {
    long long add;
    long long set_val;
    static const long long NO_SET = -2e18;
    
    RangeUpdateTag() : add(0), set_val(NO_SET) {}
    
    bool ApplyTo(RangeUpdateInfo& node, int l, int r) const {
        long long len = r - l;
        if (set_val != NO_SET) {
            node.sum = (set_val + add) * len;
            node.min_val = set_val + add;
            node.max_val = set_val + add;
        } else {
            node.sum += add * len;
            node.min_val += add;
            node.max_val += add;
        }
        return true;
    }
    
    void ApplyTo(RangeUpdateTag& other) const {
        if (set_val != NO_SET) {
            other.set_val = set_val;
            other.add = add;
        } else {
            other.add += add;
        }
    }
    
    bool Empty() const {
        return add == 0 && set_val == NO_SET;
    }

#ifdef GRACIE
    std::string print() const {
        std::ostringstream os;
        if (set_val != NO_SET) os << "set:" << set_val;
        if (add != 0) os << (set_val != NO_SET ? " " : "") << "+:" << add;
        return os.str();
    }
#endif
};

void test_cses_subarray_sum() {
    vector<SubarraySumInfo> initial;
    mt19937 rng(555);
    for (int i = 0; i < 32; i++) {
        uniform_int_distribution<long long> unif(-100, 100);
        initial.push_back(SubarraySumInfo(unif(rng)));
    }
    SimpleSegmentTree<SubarraySumInfo> st(initial);
    dbg("CSES Subarray Sum Scaling", st);
}

void test_cses_range_updates() {
    vector<RangeUpdateInfo> initial;
    for (int i = 0; i < 32; i++) {
        initial.push_back(RangeUpdateInfo(i + 1));
    }
    LazySegmentTree<RangeUpdateInfo, RangeUpdateTag> st(initial);
    
    // Prints initial state before tags
    dbg("Initial CSES Range Updates Scaling", st);

    RangeUpdateTag tag;
    tag.add = 5;
    // Modify [0, 16) maps perfectly to the entire left child intermediate node!
    st.Modify(0, 16, tag); 
    
    RangeUpdateTag set_tag;
    set_tag.set_val = 10;
    // Modify [8, 24) overlaps the middle, pushing some tags downwards while keeping others intermediate
    st.Modify(8, 24, set_tag);
    
    dbg("Tagged CSES Range Updates Scaling", st);
}

int main() {
    test_cses_subarray_sum();
    test_cses_range_updates();
    assert(true);
    return 0;
}
