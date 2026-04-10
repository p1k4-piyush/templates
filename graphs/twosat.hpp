class twosat {
public:
    digraph<int> g;
    int n;

    twosat(int _n)
        : g(digraph<int>(2 * _n))
        , n(_n)
    {
    }

    // (v[x] == value_x)
    inline void add(int x, int value_x)
    {
        assert(0 <= x && x < n);
        assert(0 <= value_x && value_x <= 1);
        g.add(2 * x + (value_x ^ 1), 2 * x + value_x);
    }

    // (v[x] == value_x || v[y] == value_y)
    inline void add(int x, int value_x, int y, int value_y)
    {
        assert(0 <= x && x < n && 0 <= y && y < n);
        assert(0 <= value_x && value_x <= 1 && 0 <= value_y && value_y <= 1);
        g.add(2 * x + (value_x ^ 1), 2 * y + value_y);
        g.add(2 * y + (value_y ^ 1), 2 * x + value_x);
    }

    inline vector<int> solve()
    {
        int cnt;
        vector<int> c = find_scc(g, cnt);
        vector<int> res(n);
        for (int i = 0; i < n; i++) {
            if (c[2 * i] == c[2 * i + 1]) {
                return vector<int>();
            }
            res[i] = (c[2 * i] < c[2 * i + 1]);
        }
        return res;
    }
};

#ifdef GRACIE
inline std::string graphviz(const twosat& ts)
{
    std::ostringstream out;
    out << "digraph G {\n";
    out << "  rankdir=LR;\n";
    out << "  nodesep=1.2;\n";
    out << "  ranksep=2.0;\n";
    out << "  splines=true;\n";
    out << "  node [shape=circle, margin=0.03, fontsize=12];\n";
    out << "  edge [arrowsize=0.5, penwidth=1.0];\n";
    for (int i = 0; i < ts.n; i++) {
        out << "  subgraph cluster_" << i << " {\n";
        out << "    style=rounded; color=gray90; bgcolor=ghostwhite;\n";
        out << "    " << 2 * i << " [label=<&not;x<sub>" << i << "</sub>>];\n";
        out << "    " << 2 * i + 1 << " [label=<x<sub>" << i << "</sub>>];\n";
        out << "  }\n";
    }
    for (int i = 0; i < 2 * ts.n; i++) {
        for (int id : ts.g.g[i]) {
            int to = ts.g.edges[id].to;
            out << "  " << i << " -> " << to << ";\n";
        }
    }
    out << "}\n";
    return out.str();
}

inline std::string graphviz_twosat(const twosat& ts, const std::vector<int>& res)
{
    std::ostringstream out;
    out << "digraph G {\n";
    out << "  rankdir=LR;\n";
    out << "  nodesep=1.2;\n";
    out << "  ranksep=2.0;\n";
    out << "  splines=true;\n";
    out << "  node [shape=circle, margin=0.03, fontsize=12];\n";
    out << "  edge [arrowsize=0.5, penwidth=1.0];\n";
    for (int i = 0; i < ts.n; i++) {
        int val = res[i];
        out << "  subgraph cluster_" << i << " {\n";
        out << "    style=rounded; color=gray90; bgcolor=ghostwhite;\n";
        out << "    " << 2 * i << " [label=<&not;x<sub>" << i << "</sub>>, style=filled, fillcolor=" << (val == 0 ? "\"lightgreen\"" : "\"lightcoral\"") << "];\n";
        out << "    " << 2 * i + 1 << " [label=<x<sub>" << i << "</sub>>, style=filled, fillcolor=" << (val == 1 ? "\"lightgreen\"" : "\"lightcoral\"") << "];\n";
        out << "  }\n";
    }
    for (int i = 0; i < 2 * ts.n; i++) {
        for (int id : ts.g.g[i]) {
            int to = ts.g.edges[id].to;
            out << "  " << i << " -> " << to << ";\n";
        }
    }
    out << "}\n";
    return out.str();
}
#endif
