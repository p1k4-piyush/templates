

template <typename T>
vector<T> dijkstra(const graph<T>& g, int start)
{
    assert(0 <= start && start < g.n);
    vector<T> dist(g.n, numeric_limits<T>::max());
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> s;
    dist[start] = 0;
    s.emplace(dist[start], start);
    while (!s.empty()) {
        auto [expected, i] = s.top();
        s.pop();
        if (dist[i] != expected) {
            continue;
        }
        for (int id : g.g[i]) {
            auto& e = g.edges[id];
            int to = e.from ^ e.to ^ i;
            if (dist[i] + e.cost < dist[to]) {
                dist[to] = dist[i] + e.cost;
                s.emplace(dist[to], to);
            }
        }
    }
    return dist;
}

#ifdef GRACIE
template <typename T>
std::string graphviz_dijkstra(const graph<T>& g, int st, const vector<T>& dist) {
    vector<int> p_edge(g.n, -1);
    for (int i=0; i<g.n; i++) {
        if (i == st) continue;
        for (int id : g.g[i]) {
            auto& e = g.edges[id];
            int from = e.from ^ e.to ^ i;
            if (dist[from] != numeric_limits<T>::max() && dist[i] == dist[from] + e.cost) {
                p_edge[i] = id;
                break;
            }
        }
    }

    std::ostringstream out;
    out << (g.directed() ? "digraph" : "graph") << " G {\n";
    out << "  node [shape=circle, margin=0.03, fontsize=10];\n";
    for (int i = 0; i < g.n; i++) {
        out << "  " << i << " [label=\"" << i;
        if (dist[i] == numeric_limits<T>::max()) out << "\\n(inf)\"";
        else out << "\\n(d=" << dist[i] << ")\"";
        if (i == st) out << ", style=filled, fillcolor=lightgreen";
        out << "];\n";
    }
    std::set<int> path_edges(p_edge.begin(), p_edge.end());
    path_edges.erase(-1);
    for (int id = 0; id < (int)g.edges.size(); id++) {
        auto& e = g.edges[id];
        int u = e.from, v = e.to;
        if (p_edge[u] == id) {
            swap(u, v);
        }
        out << "  " << u << (g.directed() ? " -> " : " -- ") << v;
        std::ostringstream opts;
        if (e.cost != 1) opts << "label=\"" << e.cost << "\"";
        if (path_edges.count(id)) {
            if (opts.str().length() > 0) opts << ", ";
            opts << "color=\"red\", penwidth=2";
        }
        if (opts.str().length() > 0) out << " [" << opts.str() << "]";
        out << ";\n";
    }
    out << "}\n";
    return out.str();
}
#endif
