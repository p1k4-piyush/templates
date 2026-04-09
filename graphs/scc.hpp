
//	https://github.com/the-tourist/algo/

#ifdef GRACIE
template <typename T>
struct scc_result : public vector<int> {
    const digraph<T>& g;
    int cnt;
    scc_result(const vector<int>& c, const digraph<T>& _g, int _cnt) : vector<int>(c), g(_g), cnt(_cnt) {}
    
    std::string graphviz() const {
        std::ostringstream out;
        out << "digraph G {\n";
        out << "  node [shape=circle];\n";
        vector<string> colors = {"lightblue", "lightgreen", "lightcoral", "plum", "wheat", "khaki"};
        for (int i = 0; i < g.n; i++) {
            int comp = (*this)[i];
            string color = colors[comp % colors.size()];
            out << "  " << i << " [style=filled, fillcolor=" << color << "];\n";
        }
        for (int i = 0; i < g.n; i++) {
            for (int id : g.g[i]) {
                int to = g.edges[id].to;
                out << "  " << i << " -> " << to;
                if ((*this)[i] == (*this)[to]) {
                    out << " [color=red, penwidth=2]";
                }
                out << ";\n";
            }
        }
        out << "}\n";
        return out.str();
    }
    friend std::ostream& operator<<(std::ostream& os, const scc_result& s) {
        return os << "[SCC Components: " << s.cnt << "]";
    }
};
#endif

template <typename T>
#ifdef GRACIE
scc_result<T>
#else
vector<int>
#endif
find_scc(const digraph<T>& g, int& cnt)
{
    digraph<T> g_rev = g.reverse();
    vector<int> order;
    vector<bool> was(g.n, false);
    function<void(int)> dfs1 = [&](int v) {
        was[v] = true;
        for (int id : g.g[v]) {
            auto& e = g.edges[id];
            int to = e.to;
            if (!was[to]) {
                dfs1(to);
            }
        }
        order.push_back(v);
    };
    for (int i = 0; i < g.n; i++) {
        if (!was[i]) {
            dfs1(i);
        }
    }
    vector<int> c(g.n, -1);
    function<void(int)> dfs2 = [&](int v) {
        for (int id : g_rev.g[v]) {
            auto& e = g_rev.edges[id];
            int to = e.to;
            if (c[to] == -1) {
                c[to] = c[v];
                dfs2(to);
            }
        }
    };
    cnt = 0;
    for (int id = g.n - 1; id >= 0; id--) {
        int i = order[id];
        if (c[i] != -1) {
            continue;
        }
        c[i] = cnt++;
        dfs2(i);
    }
#ifdef GRACIE
    return scc_result(c, g, cnt);
#else
    return c;
#endif
    // c[i] <= c[j] for every edge i -> j
}
