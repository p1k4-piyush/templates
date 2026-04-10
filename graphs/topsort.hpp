

template <typename T>
vector<int> find_topsort(const digraph<T>& g)
{
    vector<int> deg(g.n, 0);
    for (int id = 0; id < (int)g.edges.size(); id++) {
        deg[g.edges[id].to]++;
    }
    vector<int> x;
    for (int i = 0; i < g.n; i++) {
        if (deg[i] == 0) {
            x.push_back(i);
        }
    }
    for (int ptr = 0; ptr < (int)x.size(); ptr++) {
        int i = x[ptr];
        for (int id : g.g[i]) {
            auto& e = g.edges[id];
            int to = e.to;
            if (--deg[to] == 0) {
                x.push_back(to);
            }
        }
    }
    if ((int)x.size() != g.n) {
        return vector<int>();
    }
    return x;
}

#ifdef GRACIE
template <typename T>
std::string graphviz_topsort(const digraph<T>& g, const std::vector<int>& order)
{
    std::ostringstream out;
    out << "digraph G {\n";
    out << "  rankdir=LR;\n";
    out << "  node [shape=circle, margin=0.03, fontsize=10];\n";
    vector<int> rank(g.n, -1);
    for (int i = 0; i < (int)order.size(); i++) {
        rank[order[i]] = i;
    }
    for (int i = 0; i < g.n; i++) {
        out << "  " << i << " [label=\"" << i;
        if (rank[i] != -1)
            out << "\\n(Order " << rank[i] << ")\", style=filled, fillcolor=lightgreen";
        else
            out << "\", style=filled, fillcolor=gray";
        out << "];\n";
    }
    for (int i = 0; i < g.n; i++) {
        for (int id : g.g[i]) {
            int to = g.edges[id].to;
            out << "  " << i << " -> " << to << " [color=red];\n";
        }
    }
    out << "}\n";
    return out.str();
}
#endif
