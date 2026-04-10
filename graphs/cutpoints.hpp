

template <typename T>
vector<bool> find_cutpoints(dfs_undigraph<T>& g)
{
    g.dfs_all();
    vector<bool> cutpoint(g.n, false);
    for (int i = 0; i < g.n; i++) {
        if (g.pv[i] != -1 && g.min_depth[i] >= g.depth[g.pv[i]]) {
            cutpoint[g.pv[i]] = true;
        }
    }
    vector<int> children(g.n, 0);
    for (int i = 0; i < g.n; i++) {
        if (g.pv[i] != -1) {
            children[g.pv[i]]++;
        }
    }
    for (int i = 0; i < g.n; i++) {
        if (g.pv[i] == -1 && children[i] < 2) {
            cutpoint[i] = false;
        }
    }
    return cutpoint;
}

#ifdef GRACIE
template <typename T>
std::string graphviz_cutpoints(const dfs_undigraph<T>& g, const std::vector<bool>& cutpoint) {
    std::ostringstream out;
    out << "graph G {\n";
    out << "  layout=neato;\n";
    out << "  node [shape=circle];\n";
    for (int i = 0; i < g.n; i++) {
        out << "  " << i << " [label=\"" << i << "\"";
        if (cutpoint[i]) {
            out << ", color=\"red\", penwidth=2";
        }
        out << "];\n";
    }
    for (int id = 0; id < (int)g.edges.size(); id++) {
        auto& e = g.edges[id];
        out << "  " << e.from << " -- " << e.to << ";\n";
    }
    out << "}\n";
    return out.str();
}
#endif
