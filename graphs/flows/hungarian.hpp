template <typename T>
class hungarian {
public:
    int n;
    int m;
    vector<vector<T>> a;
    vector<T> u;
    vector<T> v;
    vector<int> pa;
    vector<int> pb;
    vector<int> way;
    vector<T> minv;
    vector<bool> used;
    T inf;

    hungarian(int _n, int _m)
        : n(_n)
        , m(_m)
    {
        assert(n <= m);
        a = vector<vector<T>>(n, vector<T>(m));
        u = vector<T>(n + 1);
        v = vector<T>(m + 1);
        pa = vector<int>(n + 1, -1);
        pb = vector<int>(m + 1, -1);
        way = vector<int>(m, -1);
        minv = vector<T>(m);
        used = vector<bool>(m + 1);
        inf = numeric_limits<T>::max();
    }

    inline void add_row(int i)
    {
        fill(minv.begin(), minv.end(), inf);
        fill(used.begin(), used.end(), false);
        pb[m] = i;
        pa[i] = m;
        int j0 = m;
        do {
            used[j0] = true;
            int i0 = pb[j0];
            T delta = inf;
            int j1 = -1;
            for (int j = 0; j < m; j++) {
                if (!used[j]) {
                    T cur = a[i0][j] - u[i0] - v[j];
                    if (cur < minv[j]) {
                        minv[j] = cur;
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            for (int j = 0; j <= m; j++) {
                if (used[j]) {
                    u[pb[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
        } while (pb[j0] != -1);
        do {
            int j1 = way[j0];
            pb[j0] = pb[j1];
            pa[pb[j0]] = j0;
            j0 = j1;
        } while (j0 != m);
    }

    inline T current_score()
    {
        return -v[m];
    }

    inline T solve()
    {
        for (int i = 0; i < n; i++) {
            add_row(i);
        }
        return current_score();
    }
};

#ifdef GRACIE
template <typename T>
std::string graphviz(const hungarian<T>& h)
{
    std::ostringstream out;
    out << "graph G {\n";
    out << "  rankdir=LR;\n";
    out << "  node [shape=circle];\n";
    out << "  subgraph cluster_L {\n";
    out << "    label=\"Left (1..n)\";\n";
    for (int i = 0; i < h.n; i++)
        out << "    L" << i << " [label=\"L" << i << " (u=" << h.u[i] << ")\"];\n";
    out << "  }\n";
    out << "  subgraph cluster_R {\n";
    out << "    label=\"Right (1..m)\";\n";
    for (int j = 0; j < h.m; j++)
        out << "    R" << j << " [label=\"R" << j << " (v=" << h.v[j] << ")\"];\n";
    out << "  }\n";
    for (int i = 0; i < h.n; i++) {
        for (int j = 0; j < h.m; j++) {
            out << "  L" << i << " -- R" << j;
            std::ostringstream opts;
            if (h.a[i][j] != 1)
                opts << "label=\"" << h.a[i][j] << "\"";
            if (h.pa[i] == j) {
                if (opts.str().length() > 0)
                    opts << ", ";
                opts << "color=red, penwidth=2";
            } else {
                if (opts.str().length() > 0)
                    opts << ", ";
                opts << "style=dotted, color=gray";
            }
            if (opts.str().length() > 0)
                out << " [" << opts.str() << "]";
            out << ";\n";
        }
    }
    out << "}\n";
    return out.str();
}
#endif
