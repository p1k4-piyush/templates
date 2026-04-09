
//	https://github.com/the-tourist/algo/

#ifdef GRACIE
template <typename T>
struct bridges_result : public vector<bool> {
    dfs_undigraph<T>& g;
    bridges_result(const vector<bool>& res, dfs_undigraph<T>& _g) : vector<bool>(res), g(_g) {}
    
    std::string graphviz() const {
        std::ostringstream out;
        out << "graph G {\n";
        out << "  layout=neato;\n";
        out << "  node [shape=circle];\n";
        for (int i = 0; i < g.n; i++) {
            out << "  " << i << " [label=\"" << i << "\"];\n";
        }
        int bridge_count = 0;
        for (int id = 0; id < (int)g.edges.size(); id++) {
            auto& e = g.edges[id];
            out << "  " << e.from << " -- " << e.to;
            if ((*this)[id]) {
                out << " [color=\"red\", penwidth=3]";
                bridge_count++;
            }
            out << ";\n";
        }
        out << "}\n";
        return out.str();
    }
    friend std::ostream& operator<<(std::ostream& os, const bridges_result& b) {
        int count = 0; for (int i = 0; i < b.size(); i += 2) count += b[i];
        return os << "[Bridges Count=" << count << "]";
    }
};
#endif

template <typename T>
#ifdef GRACIE
bridges_result<T>
#else
vector<bool>
#endif
find_bridges(dfs_undigraph<T>& g)
{
    g.dfs_all();
    vector<bool> bridge(g.edges.size(), false);
    for (int i = 0; i < g.n; i++) {
        if (g.pv[i] != -1 && g.min_depth[i] == g.depth[i]) {
            bridge[g.pe[i]] = true;
        }
    }
#ifdef GRACIE
    return bridges_result<T>(bridge, g);
#else
    return bridge;
#endif
}
