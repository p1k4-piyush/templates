
//	https://github.com/the-tourist/algo/

#ifdef GRACIE
template <typename T>
struct dijkstra_result : public vector<T> {
    const graph<T>& g;
    int st;
    vector<int> p_edge;
    dijkstra_result(const vector<T>& d, const graph<T>& _g, int _st, const vector<int>& _pe) 
        : vector<T>(d), g(_g), st(_st), p_edge(_pe) {}
    
    std::string graphviz() const {
        std::ostringstream out;
        out << "digraph G {\n";
        out << "  node [shape=circle, margin=0.03, fontsize=10];\n";
        for (int i = 0; i < g.n; i++) {
            out << "  " << i << " [label=\"" << i;
            if ((*this)[i] == numeric_limits<T>::max()) out << "\\n(inf)\"";
            else out << "\\n(d=" << (*this)[i] << ")\"";
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
            out << "  " << u << " -> " << v;
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
    friend std::ostream& operator<<(std::ostream& os, const dijkstra_result& r) {
        return os << "[Dijkstra SP Tree]";
    }
};
#endif

template <typename T>
#ifdef GRACIE
dijkstra_result<T>
#else
vector<T>
#endif
dijkstra(const graph<T>& g, int start)
{
    assert(0 <= start && start < g.n);
    vector<T> dist(g.n, numeric_limits<T>::max());
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> s;
#ifdef GRACIE
    vector<int> pe(g.n, -1);
#endif
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
#ifdef GRACIE
                pe[to] = id;
#endif
                s.emplace(dist[to], to);
            }
        }
    }
    
#ifdef GRACIE
    return dijkstra_result<T>(dist, g, start, pe);
#else
    return dist;
#endif
    // returns numeric_limits<T>::max() if there's no path
}
