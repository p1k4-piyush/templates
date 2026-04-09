class matching {
 public:
  vector<vector<int>> g;
  vector<int> pa;
  vector<int> pb;
  vector<int> was;
  int n, m;
  int res;
  int iter;

  matching(int _n, int _m) : n(_n), m(_m) {
    assert(0 <= n && 0 <= m);
    pa = vector<int>(n, -1);
    pb = vector<int>(m, -1);
    was = vector<int>(n, 0);
    g.resize(n);
    res = 0;
    iter = 0;
  }

  void add(int from, int to) {
    assert(0 <= from && from < n && 0 <= to && to < m);
    g[from].push_back(to);
  }

  bool dfs(int v) {
    was[v] = iter;
    for (int u : g[v]) {
      if (pb[u] == -1) {
        pa[v] = u;
        pb[u] = v;
        return true;
      }
    }
    for (int u : g[v]) {
      if (was[pb[u]] != iter && dfs(pb[u])) {
        pa[v] = u;
        pb[u] = v;
        return true;
      }
    }
    return false;
  }

#ifdef GRACIE
  std::string graphviz() const {
    std::ostringstream out;
    out << "graph G {\n";
    out << "  rankdir=LR;\n";
    out << "  node [shape=circle];\n";
    out << "  subgraph cluster_L {\n";
    out << "    label=\"Left (0.." << n - 1 << ")\";\n";
    for (int i = 0; i < n; i++) out << "    L" << i << " [label=\"L" << i << "\"];\n";
    out << "  }\n";
    out << "  subgraph cluster_R {\n";
    out << "    label=\"Right (0.." << m - 1 << ")\";\n";
    for (int j = 0; j < m; j++) out << "    R" << j << " [label=\"R" << j << "\"];\n";
    out << "  }\n";
    std::set<std::pair<int, int>> drawn;
    for (int i = 0; i < n; i++) {
      for (int u : g[i]) {
        if (drawn.insert({i, u}).second) {
          bool active = (pa[i] == u);
          out << "  L" << i << " -- R" << u;
          if (active) out << " [color=blue, penwidth=2]";
          else out << " [style=dotted, color=gray]";
          out << ";\n";
        }
      }
    }
    out << "}\n";
    return out.str();
  }
  friend std::ostream& operator<<(std::ostream& os, const matching& r) {
      return os << "Max Match: " << r.res;
  }
#endif
  int solve() {
    mt19937_64 rng(uint32_t(chrono::steady_clock::now().time_since_epoch().count()));
    for (int i = 0; i < n; i++) {
      ranges::shuffle(g[i], rng);
    }
    while (true) {
      iter++;
      int add = 0;
      for (int i = 0; i < n; i++) {
        if (pa[i] == -1 && dfs(i)) {
          add++;
        }
      }
      if (add == 0) {
        break;
      }
      res += add;
    }
    return res;
  }

  int run_one(int v) {
    if (pa[v] != -1) {
      return 0;
    }
    iter++;
    return (int) dfs(v);
  }
};
