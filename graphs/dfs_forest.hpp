

template <typename T>
class dfs_forest : public forest<T> {
public:
    using forest<T>::edges;
    using forest<T>::g;
    using forest<T>::n;

    vector<int> pv;
    vector<int> pe;
    vector<int> order;
    vector<int> pos;
    vector<int> end;
    vector<int> sz;
    vector<int> root;
    vector<int> depth;
    vector<T> dist;

    dfs_forest(int _n)
        : forest<T>(_n)
    {
    }

    void init()
    {
        pv = vector<int>(n, -1);
        pe = vector<int>(n, -1);
        order.clear();
        pos = vector<int>(n, -1);
        end = vector<int>(n, -1);
        sz = vector<int>(n, 0);
        root = vector<int>(n, -1);
        depth = vector<int>(n, -1);
        dist = vector<T>(n);
    }

    void clear()
    {
        pv.clear();
        pe.clear();
        order.clear();
        pos.clear();
        end.clear();
        sz.clear();
        root.clear();
        depth.clear();
        dist.clear();
    }

private:
    void do_dfs(int v)
    {
        pos[v] = (int)order.size();
        order.push_back(v);
        sz[v] = 1;
        for (int id : g[v]) {
            if (id == pe[v]) {
                continue;
            }
            auto& e = edges[id];
            int to = e.from ^ e.to ^ v;
            depth[to] = depth[v] + 1;
            dist[to] = dist[v] + e.cost;
            pv[to] = v;
            pe[to] = id;
            root[to] = (root[v] != -1 ? root[v] : to);
            do_dfs(to);
            sz[v] += sz[to];
        }
        end[v] = (int)order.size() - 1;
    }

    void do_dfs_from(int v)
    {
        depth[v] = 0;
        dist[v] = T {};
        root[v] = v;
        pv[v] = pe[v] = -1;
        do_dfs(v);
    }

public:
    void dfs(int v, bool clear_order = true)
    {
        if (pv.empty()) {
            init();
        } else {
            if (clear_order) {
                order.clear();
            }
        }
        do_dfs_from(v);
    }

    void dfs_all()
    {
        init();
        for (int v = 0; v < n; v++) {
            if (depth[v] == -1) {
                do_dfs_from(v);
            }
        }
        assert((int)order.size() == n);
    }
};

#ifdef GRACIE
template <typename T>
std::string graphviz(const dfs_forest<T>& g, std::string mode = "dist")
{
    std::ostringstream out;
    out << "graph G {\n";
    out << "  node [shape=box, style=rounded, fontsize=10, margin=\"0.05,0.05\"];\n";
    for (int i = 0; i < g.n; i++) {
        out << "  " << i << " [label=\"" << i;
        if (!g.pv.empty() && g.depth[i] != -1) {
            if (mode == "dist")
                out << "\\ndist=" << g.dist[i];
            else if (mode == "sz")
                out << "\\nsz=" << g.sz[i];
            else if (mode == "depth")
                out << "\\ndepth=" << g.depth[i];
            else if (mode == "pos")
                out << "\\npos=" << g.pos[i];
            else if (mode == "root")
                out << "\\nroot=" << g.root[i];
        }
        out << "\"";
        if (!g.pv.empty() && g.depth[i] == 0)
            out << ", style=\"filled,rounded\", fillcolor=\"burlywood\"";
        else if (!g.pv.empty() && g.sz[i] == 1)
            out << ", style=\"filled,rounded\", fillcolor=\"lightgreen\"";
        out << "];\n";
    }
    for (int id = 0; id < (int)g.edges.size(); id++) {
        auto& e = g.edges[id];
        out << "  " << e.from << " -- " << e.to << " [penwidth=2];\n";
    }
    out << "}\n";
    return out.str();
}
#endif
