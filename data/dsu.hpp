
class dsu {
public:
    vector<int> p;
    int n;

    dsu(int _n)
        : n(_n)
    {
        p.resize(n);
        iota(p.begin(), p.end(), 0);
    }

    inline int get(int x)
    {
        return (x == p[x] ? x : (p[x] = get(p[x])));
    }

    inline bool unite(int x, int y)
    {
        x = get(x);
        y = get(y);
        if (x != y) {
            p[x] = y;
            return true;
        }
        return false;
    }

#ifdef GRACIE
    std::string graphviz() const {
        std::ostringstream out;
        out << "digraph G {\n";
        out << "  node [shape=circle, margin=0.03, fontsize=10];\n";
        
        std::map<int, std::string> root_colors;
        const std::string colors[] = { 
            "firebrick", "dodgerblue", "forestgreen", "darkorchid", "darkorange", 
            "deeppink", "saddlebrown", "teal", "navy", "crimson", "purple", "seagreen" 
        };
        int color_idx = 0;
        
        auto get_root = [&](int x) {
            while (x != p[x]) x = p[x];
            return x;
        };

        for (int i = 0; i < n; i++) {
            int r = get_root(i);
            if (root_colors.find(r) == root_colors.end()) {
                root_colors[r] = colors[color_idx % 12];
                color_idx++;
            }
        }
        
        for (int i = 0; i < n; i++) {
            int r = get_root(i);
            out << "  " << i << " [color=\"" << root_colors[r] << "\", fontcolor=\"" << root_colors[r] << "\", penwidth=2];\n";
            if (p[i] != i) {
                out << "  " << i << " -> " << p[i] << " [color=\"" << root_colors[r] << "\", penwidth=2];\n";
            }
        }
        out << "}\n";
        return out.str();
    }
#endif
};
