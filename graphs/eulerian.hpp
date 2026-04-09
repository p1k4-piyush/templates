
//	https://github.com/the-tourist/algo/

#ifdef GRACIE
template <typename T>
struct eulerian_result : public vector<int> {
    const graph<T>& g;
    int root;
    eulerian_result(const vector<int>& res, const graph<T>& _g, int _root) : vector<int>(res), g(_g), root(_root) {}
    
    std::string graphviz() const {
        std::ostringstream out;
        out << "digraph G {\n";
        out << "  layout=neato;\n";
        out << "  node [shape=circle, style=filled, fillcolor=\"#F0F8FF\"];\n";
        for (int i = 0; i < g.n; i++) {
            out << "  " << i << " [label=\"" << i << "\"";
            if (i == root) out << ", style=filled, fillcolor=lightgreen";
            out << "];\n";
        }
        vector<int> edge_order(g.edges.size(), -1);
        for (int i = 0; i < (int)this->size(); i++) {
            edge_order[(*this)[i]] = i;
        }
        for (int id = 0; id < (int)g.edges.size(); id++) {
            auto& e = g.edges[id];
            out << "  " << e.from << " -> " << e.to << " [label=\"";
            if (edge_order[id] != -1) {
                out << edge_order[id];
            }
            out << "\"";
            if (edge_order[id] != -1) out << ", color=\"red\", penwidth=2";
            out << "];\n";
        }
        out << "}\n";
        return out.str();
    }
    friend std::ostream& operator<<(std::ostream& os, const eulerian_result& e) {
        return os << "[Eulerian Path Valid=" << (!e.empty() || e.root != -1) << "]";
    }
};
#endif

template <typename T>
#ifdef GRACIE
eulerian_result<T>
#else
vector<int>
#endif
find_eulerian_path(const graph<T>& g, int& root)
{
    // in_deg and out_deg are fake for undigraph!
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
#ifdef GRACIE
        return eulerian_result<T>(vector<int>(), g, root);
#else
        return vector<int>();
#endif
    }
    if (root == -1) {
        root = 0;
        while (root < g.n && in_deg[root] + out_deg[root] == 0) {
            root++;
        }
        if (root == g.n) {
            // an empty path
            root = 0;
#ifdef GRACIE
            return eulerian_result<T>(vector<int>(), g, root);
#else
            return vector<int>();
#endif
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
#ifdef GRACIE
    if (write_ptr != 0 || disbalance > 2) {
        root = -1;
        return eulerian_result<T>(vector<int>(), g, root);
    }
    return eulerian_result<T>(res, g, root);
#else
    if (write_ptr != 0 || disbalance > 2) {
        root = -1;
        return vector<int>();
    }
    return res;
#endif
    // returns edge ids in the path (or the cycle if it exists)
    // root == -1 if there is no path
    // (or res.empty(), but this is also true when there are no edges)
}
