

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

    virtual std::string graphviz(const std::vector<int>& path = {}, bool directed = true) const {
        return "";
    }

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

        for (auto const& e : gr.edges) {
            os << "  " << e.from << " " << edge_op << " " << e.to;
            std::string lab = gr.to_string(e);
            std::vector<std::string> attrs;
            if (!lab.empty() && lab != " ") {
                attrs.push_back("label=\"" + quote_and_escape(lab) + "\"");
                attrs.push_back("fontsize=9");
            } else if (lab.empty()) {
                if constexpr (requires { e.cost; }) {
                    if (e.cost != T(1)) {
                        std::ostringstream tmp;
                        tmp << e.cost;
                        attrs.push_back("label=\"" + quote_and_escape(tmp.str()) + "\"");
                        attrs.push_back("fontsize=9");
                    }
                }
            }
            if (!attrs.empty()) {
                os << " [";
                for (size_t i = 0; i < attrs.size(); i++) {
                    if (i > 0) os << ", ";
                    os << attrs[i];
                }
                os << "]";
            }
            os << ";\n";
        }

        os << "}\n";
        return os;
    }
};

#ifdef GRACIE
template <typename T>
std::string graphviz(const graph<T>& gr) {
    std::ostringstream out;
    out << (gr.directed() ? "digraph" : "graph") << " G {\n";
    out << "  node [shape=circle];\n";
    for (int i = 0; i < gr.n; ++i) {
        out << "  " << i << " [label=\"" << i << "\"];\n";
    }
    for (const auto& e : gr.edges) {
        out << "  " << e.from << (gr.directed() ? " -> " : " -- ") << e.to;
        if (e.cost != 1) out << " [label=\"" << e.cost << "\"]";
        out << ";\n";
    }
    out << "}\n";
    return out.str();
}
#endif
