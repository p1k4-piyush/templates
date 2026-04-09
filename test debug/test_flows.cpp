#include <bits/stdc++.h>
using namespace std;

#include "../template/dbg.hpp"
#include "../graphs/graph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/forest.hpp"
#include "../graphs/flows/flow_graph.hpp"
#include "../graphs/flows/dinic.hpp"
#include "../graphs/flows/mcmf.hpp"
#include "../graphs/flows/fastflow.hpp"
#include "../graphs/flows/flow_decomposition.hpp"
#include "../graphs/flows/hungarian.hpp"
#include "../graphs/flows/matching.hpp"
#include "../graphs/flows/blossom.hpp"
#include "../graphs/flows/mincut.hpp"
#include "../graphs/flows/gomory_hu.hpp"

using namespace std;

void test_dinic_fastflow() {
    flow_graph<long long> fg(6, 0, 5);
    fg.add(0, 1, 16, 0);
    fg.add(0, 2, 13, 0);
    fg.add(1, 2, 10, 4); // Added initial backward cap for variation
    fg.add(1, 3, 12, 0);
    fg.add(2, 1, 4, 0);
    fg.add(2, 4, 14, 0);
    fg.add(3, 2, 9, 0);
    fg.add(3, 5, 20, 0);
    fg.add(4, 3, 7, 0);
    fg.add(4, 5, 4, 0);

    dinic<long long> din(fg);
    long long ans = din.max_flow();
    dbg("Dinic Layered Network Layout", fg); // Implicitly uses flow_graph rendering!
    
    auto cut = din.min_cut();
    dbg("Dinic S-T Min Cut", cut);

    // Fast Flow Test
    fastflow<long long> ff(6, 0, 5);
    ff.add(0, 1, 16, 0);
    ff.add(0, 2, 13, 0);
    ff.add(1, 2, 10, 4); 
    ff.add(1, 3, 12, 0);
    ff.add(2, 1, 4, 0);
    ff.add(2, 4, 14, 0);
    ff.add(3, 2, 9, 0);
    ff.add(3, 5, 20, 0);
    ff.add(4, 3, 7, 0);
    ff.add(4, 5, 4, 0);
    
    ff.max_flow();
    auto fast_cut = ff.min_cut();
    dbg("ISAP FastFlow Network", fast_cut); // Renders gap structures d[i] natively!

    // Flow Decomposition (works on Dinic outputs)
    flow_decomposition<long long> decomp(fg);
    decomp.decompose();
    dbg("Greedy Flow Decomposition", decomp);
}

void test_mcmf() {
    MCMF<long long, long long> mcmf(4);
    mcmf.add(0, 1, 2, 0, 1);
    mcmf.add(0, 2, 1, 0, 2);
    mcmf.add(1, 2, 1, 0, -2); // Negative cost cycle possibility mapping handled
    mcmf.add(1, 3, 1, 0, 1);
    mcmf.add(2, 3, 2, 0, 4);

    auto result = mcmf.max_flow_min_cost(0, 3);
    dbg("Min Cost Max Flow Successive Path", mcmf);
}

void test_matching_algorithms() {
    hungarian<int> h(3, 4); // Bipartite 3 -> 4
    h.a = {
        {5, 2, 1, 8},
        {4, 10, 2, 3},
        {6, 7, 3, 1}
    };
    h.solve();
    dbg("Hungarian Minimal Assignment", h);

    matching mx(4, 5);
    mx.add(0, 1); mx.add(0, 2);
    mx.add(1, 2); mx.add(1, 3);
    mx.add(2, 1); mx.add(2, 4);
    mx.add(3, 4);
    mx.solve();
    dbg("Hopcroft Bipartite Matching", mx);

    undigraph<int> blossom_ug(6);
    blossom_ug.add(0, 1);
    blossom_ug.add(1, 2);
    blossom_ug.add(2, 0); // Odd cycle (triangle) -> requires blossom shrinking
    blossom_ug.add(2, 3);
    blossom_ug.add(3, 4);
    blossom_ug.add(4, 5);
    
    auto shrink = find_max_unweighted_matching(blossom_ug);
    dbg("Edmonds Blossom Matching Shrink", shrink);
}

void test_global_cuts() {
    vector<vector<int>> st_graph = {
        {0, 2, 3, 0},
        {2, 0, 1, 4},
        {3, 1, 0, 2},
        {0, 4, 2, 0}
    };
    auto stoer = MinCut(st_graph);
    dbg("Stoer Wagner Global Min Cut", stoer);

    undigraph<long long> ghu(6);
    ghu.add(0, 1, 1); ghu.add(0, 2, 2);
    ghu.add(1, 2, 3); ghu.add(1, 3, 4);
    ghu.add(2, 4, 5); ghu.add(3, 4, 6);
    ghu.add(3, 5, 7); ghu.add(4, 5, 8);
    auto ht = gomory_hu(ghu);
    dbg("Gomory Hu Connected Trees", ht);
}

int main() {
    test_dinic_fastflow();
    test_mcmf();
    test_matching_algorithms();
    test_global_cuts();
    return 0;
}
