
using namespace std;

#ifdef GRACIE
template <typename T>
struct mincut_result : public pair<T, vector<bool>> {
    vector<vector<T>> orig_g;

    std::string graphviz() const {
        std::ostringstream os;
        os << "graph G {\n"; 
        os << "  node [shape=circle, fontname=\"Courier\"];\n";
        
        const auto& g = orig_g;
        int n = g.size();
        for (int i = 0; i < n; ++i) {
            os << "  " << i << " [label=\"" << i << "\"";
            if (this->second[i]) {
               os << ", style=filled, fillcolor=\"#a2ff9c\"";
            } else {
               os << ", style=filled, fillcolor=\"#ffbc82\"";
            }
            os << "];\n";
        }

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (g[i][j] > 0) {
                    os << "  " << i << " -- " << j << " [label=\"" << g[i][j] << "\"";
                    if (this->second[i] != this->second[j]) { 
                        os << ", color=\"red\", style=\"dashed\", penwidth=2.5, fontcolor=\"red\"";
                    } else {
                        os << ", color=\"gray80\", fontcolor=\"gray80\"";
                    }
                    os << "];\n";
                }
            }
        }
        os << "}\n";
        return os.str();
    }
};
#else
template <typename T>
using mincut_result = pair<T, vector<bool>>;
#endif

