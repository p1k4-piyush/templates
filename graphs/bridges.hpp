

template <typename T>
vector<bool> find_bridges(dfs_undigraph<T>& g)
{
    g.dfs_all();
    vector<bool> bridge(g.edges.size(), false);
    for (int i = 0; i < g.n; i++) {
        if (g.pv[i] != -1 && g.min_depth[i] == g.depth[i]) {
            bridge[g.pe[i]] = true;
        }
    }
    return bridge;
}

#ifdef GRACIE
template <typename T>
std::string graphviz_bridges(const dfs_undigraph<T>& g, const std::vector<bool>& bridge) {
    std::ostringstream out;
    out << "graph G {\n";
    out << "  layout=neato;\n";
    out << "  node [shape=circle];\n";
    for (int i = 0; i < g.n; i++) {
        out << "  " << i << " [label=\"" << i << "\"];\n";
    }
    for (int id = 0; id < (int)g.edges.size(); id++) {
        auto& e = g.edges[id];
        out << "  " << e.from << " -- " << e.to;
        if (bridge[id]) {
            out << " [color=\"red\", penwidth=3]";
        }
        out << ";\n";
    }
    out << "}\n";
    return out.str();
}
#endif
