
//	https://github.com/the-tourist/algo/


#ifdef GRACIE
template <typename T>
struct mst_result : public vector<int> {
    const undigraph<T>& g;
    T ans;
    mst_result(const vector<int>& res, const undigraph<T>& _g, T _ans) : vector<int>(res), g(_g), ans(_ans) {}
    
    std::string graphviz() const {
        std::ostringstream out;
        out << "graph G {\n";
        out << "  layout=neato;\n";
        out << "  node [shape=circle];\n";
        for (int i = 0; i < g.n; i++) {
            out << "  " << i << " [label=\"" << i << "\"];\n";
        }
        std::set<int> mst_edges(this->begin(), this->end());
        for (int id = 0; id < (int)g.edges.size(); id++) {
            auto& e = g.edges[id];
            out << "  " << e.from << " -- " << e.to;
            std::ostringstream opts;
            if (e.cost != 1) opts << "label=\"" << e.cost << "\"";
            if (mst_edges.count(id)) {
                if (opts.str().length() > 0) opts << ", ";
                opts << "color=\"red\", penwidth=2";
            }
            if (opts.str().length() > 0) out << " [" << opts.str() << "]";
            out << ";\n";
        }
        out << "}\n";
        return out.str();
    }
    friend std::ostream& operator<<(std::ostream& os, const mst_result& m) {
        return os << "[MST Cost=" << m.ans << " Edges=" << m.size() << "]";
    }
};
#endif

template <typename T>
#ifdef GRACIE
mst_result<T>
#else
vector<int>
#endif
find_mst(const undigraph<T>& g, T& ans)
{
    vector<int> order(g.edges.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(),
        [&g](int a, int b) { return g.edges[a].cost < g.edges[b].cost; });
    dsu d(g.n);
    vector<int> ans_list;
    ans = 0;
    for (int id : order) {
        auto& e = g.edges[id];
        if (d.get(e.from) != d.get(e.to)) {
            d.unite(e.from, e.to);
            ans_list.push_back(id);
            ans += e.cost;
        }
    }
#ifdef GRACIE
    return mst_result<T>(ans_list, g, ans);
#else
    return ans_list;
#endif
    // returns edge ids of minimum "spanning" forest
}
