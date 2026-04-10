

template <typename T>
vector<vector<int>>
find_cycles(const graph<T>& g, int bound_cnt = 1 << 30,
    int bound_size = 1 << 30)
{
    vector<int> was(g.n, -1);
    vector<int> st;
    vector<vector<int>> cycles;
    int total_size = 0;
    function<void(int, int)> dfs = [&](int v, int pe) {
        if ((int)cycles.size() >= bound_cnt || total_size >= bound_size) {
            return;
        }
        was[v] = (int)st.size();
        for (int id : g.g[v]) {
            if (id == pe) {
                continue;
            }
            auto& e = g.edges[id];
            int to = e.from ^ e.to ^ v;
            if (was[to] >= 0) {
                vector<int> cycle(1, id);
                for (int j = was[to]; j < (int)st.size(); j++) {
                    cycle.push_back(st[j]);
                }
                cycles.push_back(cycle);
                total_size += (int)cycle.size();
                if ((int)cycles.size() >= bound_cnt || total_size >= bound_size) {
                    was[v] = -2;
                    return;
                }
                continue;
            }
            if (was[to] == -1) {
                st.push_back(id);
                dfs(to, id);
                st.pop_back();
            }
        }
        was[v] = -2;
    };
    for (int i = 0; i < g.n; i++) {
        if (was[i] == -1) {
            dfs(i, -1);
        }
    }
    return cycles;
}

#ifdef GRACIE
template <typename T>
std::string graphviz_cycles(const graph<T>& g, const std::vector<std::vector<int>>& cycles) {
    std::ostringstream out;
    out << "graph G {\n";
    out << "  layout=neato;\n";
    out << "  node [shape=circle, style=filled, fillcolor=\"#F0F8FF\"];\n";
    for (int i = 0; i < g.n; i++) {
        out << "  " << i << " [label=\"" << i << "\"];\n";
    }
    vector<string> colors = {"red", "green", "blue", "magenta", "orange", "purple", "brown", "cyan"};
    std::map<int, vector<pair<int, string>>> cycle_edge_colors;
    int color_idx = 0;
    for (const auto& cycle : cycles) {
        string color = colors[color_idx % colors.size()];
        for (int id : cycle) cycle_edge_colors[id].push_back({color_idx, color});
        color_idx++;
    }
    for (int id = 0; id < (int)g.edges.size(); id++) {
        auto& e = g.edges[id];
        out << "  " << e.from << " -- " << e.to;
        if (cycle_edge_colors.count(id)) {
            out << " [color=\"";
            const auto& cols = cycle_edge_colors[id];
            for (size_t i = 0; i < cols.size(); ++i) {
                out << cols[i].second << (i + 1 == cols.size() ? "" : ":invis:");
            }
            out << "\", penwidth=2";
            if (cols.size() > 3) {
                out << ", label=<";
                out << "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"1\" color=\"black\"><tr>";
                for (size_t i = 0; i < cols.size(); ++i) {
                    out << "<td bgcolor=\"white\"><font point-size=\"10\" color=\"" << cols[i].second << "\"><b>C" << cols[i].first << "</b></font></td>";
                }
                out << "</tr></table>";
                out << ">";
            }
            out << "]";
        }
        out << ";\n";
    }
    out << "}\n";
    return out.str();
}
#endif

template <typename T>
vector<int> edges_to_vertices(const graph<T>& g, const vector<int>& edge_cycle)
{
    int sz = (int)edge_cycle.size();
    vector<int> vertex_cycle;
    if (sz <= 2) {
        vertex_cycle.push_back(g.edges[edge_cycle[0]].from);
        if (sz == 2) {
            vertex_cycle.push_back(g.edges[edge_cycle[0]].to);
        }
    } else {
        for (int i = 0; i < sz; i++) {
            int j = (i + 1) % sz;
            auto& e = g.edges[edge_cycle[i]];
            auto& other = g.edges[edge_cycle[j]];
            if (other.from == e.from || other.to == e.from) {
                vertex_cycle.push_back(e.to);
            } else {
                vertex_cycle.push_back(e.from);
            }
        }
    }
    return vertex_cycle;
    // only for simple cycles!
}
