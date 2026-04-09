
//	https://github.com/the-tourist/algo/

#ifdef GRACIE
template <typename T>
struct cutpoints_result : public vector<bool> {
    dfs_undigraph<T>& g;
    cutpoints_result(const vector<bool>& res, dfs_undigraph<T>& _g) : vector<bool>(res), g(_g) {}
    
    std::string graphviz() const {
        std::ostringstream out;
        out << "graph G {\n";
        out << "  layout=neato;\n";
        out << "  node [shape=circle];\n";
        int cp_count = 0;
        for (int i = 0; i < g.n; i++) {
            out << "  " << i << " [label=\"" << i << "\"";
            if ((*this)[i]) {
                out << ", color=\"red\", penwidth=2";
                cp_count++;
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
    friend std::ostream& operator<<(std::ostream& os, const cutpoints_result& c) {
        int count = 0; for (int i = 0; i < c.size(); i++) count += c[i];
        return os << "[Cutpoints Count=" << count << "]";
    }
};
#endif

template <typename T>
#ifdef GRACIE
cutpoints_result<T>
#else
vector<bool>
#endif
find_cutpoints(dfs_undigraph<T>& g)
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
#ifdef GRACIE
    return cutpoints_result<T>(cutpoint, g);
#else
    return cutpoint;
#endif
}
