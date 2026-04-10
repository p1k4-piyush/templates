

template <typename T>
vector<int> find_eulerian_path(const graph<T>& g, int& root)
{
    vector<int> in_deg(g.n, 0);
    vector<int> out_deg(g.n, 0);
    int cnt_edges = 0;
    for (int id = 0; id < (int)g.edges.size(); id++) {
        cnt_edges++;
        auto& e = g.edges[id];
        out_deg[e.from]++;
        in_deg[e.to]++;
    }
    root = -1;
    int odd = 0;
    for (int i = 0; i < g.n; i++) {
        if ((in_deg[i] + out_deg[i]) % 2 == 1) {
            odd++;
            if (root == -1 || out_deg[i] - in_deg[i] > out_deg[root] - in_deg[root]) {
                root = i;
            }
        }
    }
    if (odd > 2) {
        root = -1;
        return vector<int>();
    }
    if (root == -1) {
        root = 0;
        while (root < g.n && in_deg[root] + out_deg[root] == 0) {
            root++;
        }
        if (root == g.n) {
            root = 0;
            return vector<int>();
        }
    }
    vector<bool> used(g.edges.size(), false);
    vector<int> ptr(g.n, 0);
    vector<int> balance(g.n, 0);
    vector<int> res(cnt_edges);
    int stack_ptr = 0;
    int write_ptr = cnt_edges;
    int v = root;
    while (true) {
        bool found = false;
        while (ptr[v] < (int)g.g[v].size()) {
            int id = g.g[v][ptr[v]++];
            if (used[id]) {
                continue;
            }
            used[id] = true;
            res[stack_ptr++] = id;
            auto& e = g.edges[id];
            balance[v]++;
            v ^= e.from ^ e.to;
            balance[v]--;
            found = true;
            break;
        }
        if (!found) {
            if (stack_ptr == 0) {
                break;
            }
            int id = res[--stack_ptr];
            res[--write_ptr] = id;
            auto& e = g.edges[id];
            v ^= e.from ^ e.to;
        }
    }
    int disbalance = 0;
    for (int i = 0; i < g.n; i++) {
        disbalance += abs(balance[i]);
    }
    if (write_ptr != 0 || disbalance > 2) {
        root = -1;
        return vector<int>();
    }
    return res;
}

#ifdef GRACIE
template <typename T>
std::string graphviz_eulerian(const graph<T>& g, const std::vector<int>& path, int root)
{
    std::ostringstream out;
    out << "digraph G {\n";
    out << "  layout=neato;\n";
    out << "  overlap=false;\n";
    out << "  splines=true;\n";
    out << "  node [shape=circle, style=filled, fillcolor=\"#F0F8FF\"];\n";
    for (int i = 0; i < g.n; i++) {
        out << "  " << i << " [label=\"" << i << "\"";
        if (i == root)
            out << ", style=filled, fillcolor=lightgreen";
        out << "];\n";
    }
    vector<int> edge_order(g.edges.size(), -1);
    for (int i = 0; i < (int)path.size(); i++) {
        edge_order[path[i]] = i;
    }
    for (int id = 0; id < (int)g.edges.size(); id++) {
        auto& e = g.edges[id];
        out << "  " << e.from << " -> " << e.to << " [label=\"";
        if (edge_order[id] != -1) {
            out << edge_order[id];
        }
        out << "\"";
        if (edge_order[id] != -1)
            out << ", fontsize=16, arrowsize=0.5";
        out << "];\n";
    }
    out << "}\n";
    return out.str();
}
#endif
