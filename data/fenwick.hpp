
// https://github.com/the-tourist/algo/blob/master/data/fenwick.cpp

template <typename T>
class FenwickTree {
public:
    vector<T> fenw;
    int n;
    int pw;

    FenwickTree()
        : n(0)
    {
    }
    FenwickTree(int n_)
        : n(n_)
    {
        fenw.resize(n);
        pw = bit_floor(unsigned(n));
    }

    void Modify(int x, T v)
    {
        assert(0 <= x && x <= n);
        while (x < n) {
            fenw[x] += v;
            x |= x + 1;
        }
    }

    T Query(int x)
    {
        assert(0 <= x && x <= n);
        T v {};
        while (x > 0) {
            v += fenw[x - 1];
            x &= x - 1;
        }
        return v;
    }

    // Returns the length of the longest prefix with sum <= c
    int MaxPrefix(T c)
    {
        T v {};
        int at = 0;
        for (int len = pw; len > 0; len >>= 1) {
            if (at + len <= n) {
                auto nv = v;
                nv += fenw[at + len - 1];
                if (!(c < nv)) {
                    v = nv;
                    at += len;
                }
            }
        }
        assert(0 <= at && at <= n);
        return at;
    }

#ifdef GRACIE
    std::string graphviz() const {
        std::ostringstream out;
        out << "digraph G {\n";
        out << "  layout=dot;\n";
        out << "  node [shape=box];\n";
        
        // Draw leaves (Original Array)
        for (int i = 0; i < n; i++) {
            out << "  L" << i << " [shape=none, margin=0, label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\"><tr><td bgcolor=\"#F0F8FF\">A[" << i << "]</td></tr></table>>];\n";
        }
        
        // Draw Fenwick Nodes
        for (int i = 0; i < n; i++) {
            out << "  F" << i << " [shape=none, margin=0, label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\">";
            out << "<tr><td bgcolor=\"#FFDAB9\">F[" << i << "]</td></tr>";
            out << "<tr><td bgcolor=\"#FFFFE0\">Val: "; 
            _dbglib::gracie_safe_print(out, fenw[i]); 
            out << "</td></tr></table>>];\n";
            
            // Connect Parent Fenwick Node to Leaf
            out << "  F" << i << " -> L" << i << " [color=blue];\n";
            
            // Connect previous Fenwick nodes that conceptually sum into this one
            int V = i + 1;
            int lowest_bit = V & -V;
            for (int step = 1; step < lowest_bit; step <<= 1) {
                int child_V = V - step;
                // Form correct hierarchical Top-Down visual layout
                out << "  F" << i << " -> F" << (child_V - 1) << " [color=blue];\n";
            }
        }
        
        out << "}\n";
        return out.str();
    }
    friend std::ostream& operator<<(std::ostream& os, const FenwickTree& f) {
        return os << "[Fenwick Tree size=" << f.n << "]";
    }
#endif
};
