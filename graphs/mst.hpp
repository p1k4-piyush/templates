

template <typename T>
vector<int> find_mst(const undigraph<T>& g, T& ans)
{
    vector<int> order(g.edges.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
        [&g](int a, int b) { return g.edges[a].cost < g.edges[b].cost; });
    dsu d(g.n);
    vector<int> ans_list;
    ans = 0;
    for (int id : order) {
        auto& e = g.edges[id];
        if (d.get(e.from) != d.get(e.to)) {
            d.unite(e.from, e.to);
            ans_list.push_back(id);
            ans += e.cost;
        }
    }
    return ans_list;
}
#ifdef GRACIE
template <typename T>
std::string graphviz_mst(const undigraph<T>& g, const std::vector<int>& mst_list)
{
    std::ostringstream out;
    out << "graph G {\n";
    out << "  layout=neato;\n";
    out << "  node [shape=circle];\n";
    for (int i = 0; i < g.n; i++) {
        out << "  " << i << " [label=\"" << i << "\"];\n";
    }
    std::set<int> mst_edges(mst_list.begin(), mst_list.end());
    for (int id = 0; id < (int)g.edges.size(); id++) {
        auto& e = g.edges[id];
        out << "  " << e.from << " -- " << e.to;
        std::ostringstream opts;
        if (e.cost != 1)
            opts << "label=\"" << e.cost << "\"";
        if (mst_edges.count(id)) {
            if (opts.str().length() > 0)
                opts << ", ";
            opts << "color=\"red\", penwidth=2";
        }
        if (opts.str().length() > 0)
            out << " [" << opts.str() << "]";
        out << ";\n";
    }
    out << "}\n";
    return out.str();
}
#endif
// returns edge ids of minimum "spanning" forest
