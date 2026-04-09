
//  https://github.com/the-tourist/algo/blob/master/data/sparsetable.cpp

template <typename T, typename F>
class SparseTable {
public:
    int n;
    vector<vector<T>> mat;
    F func;

    SparseTable(const vector<T>& a, const F& f)
        : func(f)
    {
        n = static_cast<int>(a.size());
        int max_log = 32 - __builtin_clz(n);
        mat.resize(max_log);
        mat[0] = a;
        for (int j = 1; j < max_log; j++) {
            mat[j].resize(n - (1 << j) + 1);
            for (int i = 0; i <= n - (1 << j); i++) {
                mat[j][i] = func(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    T get(int from, int to) const
    {
        assert(0 <= from && from <= to && to <= n - 1);
        int lg = 32 - __builtin_clz(to - from + 1) - 1;
        return func(mat[lg][from], mat[lg][to - (1 << lg) + 1]);
    }

#ifdef GRACIE
    std::string graphviz() const {
        std::ostringstream os;
        os << "digraph G {\n  node [shape=plaintext, fontsize=10];\n";
        os << "  st [label=<\n    <table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n";
        os << "      <tr><td bgcolor=\"#eeeeee\"><b>(lg \\\\ i)</b></td>";
        for (int j = 0; j < n; j++) os << "<td bgcolor=\"#eeeeee\"><b>" << j << "</b></td>";
        os << "</tr>\n";
        for (int j = 0; j < (int)mat.size(); j++) {
            os << "      <tr><td bgcolor=\"#eeeeee\"><b>" << j << " (len " << (1 << j) << ")</b></td>";
            for (int i = 0; i < n; i++) {
                if (i < (int)mat[j].size()) {
                    os << "<td>" << mat[j][i] << "</td>";
                } else {
                    os << "<td bgcolor=\"#cccccc\"></td>";
                }
            }
            os << "</tr>\n";
        }
        os << "    </table>\n  >];\n}\n";
        return os.str();
    }
    friend std::ostream& operator<<(std::ostream& os, const SparseTable& st) {
        return os << "[SparseTable n=" << st.n << "]";
    }
#endif
};
