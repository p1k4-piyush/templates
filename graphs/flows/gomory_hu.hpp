template <typename T>
forest<T> gomory_hu(const undigraph<T>& g)
{
    int n = g.n;
    flow_graph<T> fg(n, 0, 1);
    for (auto& e : g.edges) {
        fg.add(e.from, e.to, e.cost, e.cost);
    }
    forest<T> ret(n);
    vector<int> pr(n, 0);
    for (int i = 1; i < n; i++) {
        fg.clear_flow();
        fg.st = i;
        fg.fin = pr[i];
        dinic<T> din(fg);
        T flow = din.max_flow();
        vector<bool> cut = din.min_cut();
        for (int j = i + 1; j < n; j++) {
            if (cut[j] == cut[i] && pr[j] == pr[i]) {
                pr[j] = i;
            }
        }
        ret.add(i, pr[i], flow);
    }
    return ret;
}

#ifdef GRACIE
template <typename T>
std::string graphviz_gomory_hu(const forest<T>& f)
{
    std::ostringstream out;
    out << "graph G {\n";
    out << "  node [shape=circle, style=filled, fillcolor=lightblue];\n";
    for (int i = 0; i < f.n; i++)
        out << "  " << i << " [label=\"" << i << "\"];\n";
    for (const auto& e : f.edges) {
        if (e.from < e.to) {
            out << "  " << e.from << " -- " << e.to;
            std::ostringstream opts;
            if (e.cost != 1)
                opts << "label=\"" << e.cost << "\"";
            if (opts.str().length() > 0)
                opts << ", ";
            opts << "penwidth=2";
            out << " [" << opts.str() << "];\n";
        }
    }
    out << "}\n";
    return out.str();
}
#endif
