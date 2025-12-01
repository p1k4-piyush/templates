
//	https://github.com/the-tourist/algo/

template <typename T>
class graph {
public:
    struct edge {
        int from;
        int to;
        T cost;
    };

    vector<edge> edges;
    vector<vector<int>> g;
    int n;

    graph(int _n)
        : n(_n)
    {
        g.resize(n);
    }

    virtual int add(int from, int to, T cost) = 0;

    virtual std::string to_string(int v) const
    {
        return ::to_string(v);
    }

    virtual std::string to_string(edge const& e) const
    {
        if (e.cost == T(0) || e.cost == T(1)) {
            return " ";
        }
        return ::to_string(e.cost);
    }

    virtual bool directed() const { return false; }

    friend std::ostream& operator<<(std::ostream& os, const graph<T>& gr)
    {
        auto quote_and_escape = [](std::string_view s) {
            std::string out;
            out.reserve(s.size());
            for (char c : s)
                out.push_back(c == '"' ? '\'' : c);
            return out;
        };

        const char* header = gr.directed() ? "digraph" : "graph";
        const char* edge_op = gr.directed() ? "->" : "--";

        os << header << " G {\n";
        os << "  rankdir=LR;\n";
        os << "  node [shape=circle, fontsize=10];\n";

        for (int v = 0; v < gr.n; ++v) {
            std::string lbl = gr.to_string(v);
            if (lbl.empty())
                lbl = std::to_string(v);
            os << "  " << v << " [label=\"" << quote_and_escape(lbl) << "\"];\n";
        }

        // edges (use to_string(edge) if overridden)
        for (auto const& e : gr.edges) {
            os << "  " << e.from << " " << edge_op << " " << e.to;
            std::string lab = gr.to_string(e);
            if (!lab.empty()) {
                os << " [label=\"" << quote_and_escape(lab) << "\", fontsize=9]";
            } else {
                if constexpr (requires { e.cost; }) {
                    std::ostringstream tmp;
                    tmp << e.cost;
                    os << " [label=\"" << quote_and_escape(tmp.str()) << "\", fontsize=9]";
                }
            }
            os << ";\n";
        }

        os << "}\n";
        return os;
    }
};
