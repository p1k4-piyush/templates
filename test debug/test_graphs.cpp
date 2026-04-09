#include <bits/stdc++.h>
using namespace std;

#include "../template/dbg.hpp"
#include "../graphs/graph.hpp"
#include "../graphs/digraph.hpp"
#include "../graphs/undigraph.hpp"
#include "../graphs/forest.hpp"
#include "../graphs/dfs_undigraph.hpp"
#include "../graphs/dfs_forest.hpp"

#include "../graphs/scc.hpp"
#include "../graphs/twosat.hpp"
#include "../graphs/topsort.hpp"
#include "../graphs/dijkstra.hpp"
#include "../graphs/eulerian.hpp"
#include "../data/dsu.hpp"
#include "../graphs/mst.hpp"
#include "../graphs/bridges.hpp"
#include "../graphs/cutpoints.hpp"
#include "../graphs/cycles.hpp"
#include "../maths-numeric/matrix.hpp"

void test_graphs() {
    // --- SCC ---
    digraph<int> dg(15);
    for (int i = 0; i < 14; i++) dg.add(i, i + 1);
    dg.add(4, 0); dg.add(9, 5); dg.add(14, 10); // 3 components
    int scc_cnt;
    auto sccs = find_scc(dg, scc_cnt);
    dbg("SCC Map", sccs);

    // --- TwoSat ---
    twosat ts(15);
    for (int i = 0; i < 14; i++) ts.add(i, 1, i + 1, 0);
    ts.add(0, 0, 14, 1);
    auto ts_res = ts.solve();
    dbg("TwoSat Resolution", ts_res);

    // --- Topsort ---
    digraph<int> dag(15);
    for (int i = 0; i < 14; i++) dag.add(i, i + 1);
    dag.add(0, 5); dag.add(5, 10); dag.add(2, 12);
    auto top = find_topsort(dag);
    dbg("Topological Sort", top);

    // --- Dijkstra ---
    digraph<int> wg(15);
    for (int i = 0; i < 14; i++) wg.add(i, i + 1, (i % 3) + 1);
    wg.add(0, 5, 2); wg.add(5, 14, 4);
    auto sp = dijkstra(wg, 0);
    dbg("Dijkstra SP Tree", sp);

    // --- Eulerian Path ---
    digraph<int> ep(15); // Cycle of 15 nodes
    for (int i = 0; i < 15; i++) ep.add(i, (i + 1) % 15);
    int root;
    auto epath = find_eulerian_path(ep, root);
    dbg("Eulerian Path", epath);

    // --- MST ---
    undigraph<int> u(15);
    for (int i = 0; i < 14; i++) u.add(i, i + 1, (i % 5) + 1);
    u.add(0, 7, 2); u.add(4, 12, 1); u.add(14, 0, 8);
    int mst_cost;
    auto mst_res = find_mst(u, mst_cost);
    dbg("MST Extract", mst_res);

    // --- Bridges and Cutpoints ---
    dfs_undigraph<int> du(15);
    for (int i = 0; i < 4; i++) du.add(i, i + 1); du.add(4, 0); // Cycle 1 (0-4)
    du.add(5, 6); du.add(6, 7); du.add(7, 5); // Cycle 2 (5-7)
    for (int i = 8; i < 14; i++) du.add(i, i + 1); du.add(14, 8); // Cycle 3 (8-14)
    // Bridges
    du.add(0, 5); du.add(7, 8); 
    auto bridges = find_bridges(du);
    dbg("Bridges Highlight", bridges);
    
    auto cuts = find_cutpoints(du);
    dbg("Cutpoints Highlight", cuts);

    // --- Cycles ---
    undigraph<int> cg(15);
    for (int i = 0; i < 4; i++) cg.add(i, i + 1); cg.add(4, 0);
    cg.add(4, 5); cg.add(5, 6); cg.add(6, 4);
    cg.add(6, 7); cg.add(7, 8); cg.add(8, 9); cg.add(9, 10); cg.add(10, 7);
    auto cycs = find_cycles(cg);
    dbg("Cycles Highlight", cycs);

    vector<vector<int>> mat = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
#ifdef GRACIE
    dbg("Matrix Render Native Override", visualize_matrix(mat));
#endif
}

int main() {
    test_graphs();
    return 0;
}
