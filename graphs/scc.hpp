

template <typename T>
vector<int> find_scc(const digraph<T>& g, int& cnt)
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
    return c;
}
#ifdef GRACIE
template <typename T>
std::string graphviz_scc(const digraph<T>& g, const std::vector<int>& c) {
    std::ostringstream out;
    out << "digraph G {\n";
    out << "  node [shape=circle];\n";
    vector<string> colors = {"lightblue", "lightgreen", "lightcoral", "plum", "wheat", "khaki"};
    for (int i = 0; i < g.n; i++) {
        int comp = c[i];
        string color = colors[comp % colors.size()];
        out << "  " << i << " [style=filled, fillcolor=" << color << "];\n";
    }
    for (int i = 0; i < g.n; i++) {
        for (int id : g.g[i]) {
            int to = g.edges[id].to;
            out << "  " << i << " -> " << to;
            if (c[i] == c[to]) {
                out << " [color=red, penwidth=2]";
            }
            out << ";\n";
        }
    }
    out << "}\n";
    return out.str();
}
#endif
    // c[i] <= c[j] for every edge i -> j
