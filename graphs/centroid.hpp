#ifdef GRACIE
template <typename T>
struct centroid_result : public std::vector<int> {
    const forest<T>* g_ptr;
    std::vector<int> parent_centroid;

    std::string graphviz() const {
        std::ostringstream os;
        os << "digraph G {\n  node [shape=circle, fontsize=10];\n";
        for (int i = 0; i < g_ptr->n; i++) {
           os << "  " << i << " [label=\"" << i << "\"];\n";
        }
        for (int i = 0; i < g_ptr->n; i++) {
           if (parent_centroid[i] != -1) {
               os << "  " << parent_centroid[i] << " -> " << i << " [color=red, penwidth=2.0];\n";
           }
        }
        for (auto const& e : g_ptr->edges) {
           os << "  " << e.from << " -> " << e.to << " [style=dotted, color=gray, dir=none, constraint=false];\n";
        }
        os << "}\n";
        return os.str();
    }
};
#else
template <typename T>
using centroid_result = std::vector<int>;
#endif

template <typename T>
centroid_result<T> centroid_decomposition(const forest<T>& g) {

  int n = g.n;
  vector<bool> alive(n, true);
  centroid_result<T> res;
#ifdef GRACIE
  res.g_ptr = &g;
  res.parent_centroid.assign(n, -1);
#endif
  res.reserve(n);
  vector<int> sz(n);
  auto Dfs = [&](auto&& self, int v, int pr) -> void {
    sz[v] = 1;
    for (int eid : g.g[v]) {
      auto& e = g.edges[eid];
      int u = e.from ^ e.to ^ v;
      if (u != pr && alive[u]) {
        self(self, u, v);
        sz[v] += sz[u];
      }
    }
  };
  auto Build = [&](auto&& self, int v, int parent_c) -> void {
    Dfs(Dfs, v, -1);
    int c = v;
    int pr = -1;
    while (true) {
      int nxt = -1;
      for (int eid : g.g[c]) {
        auto& e = g.edges[eid];
        int u = e.from ^ e.to ^ c;
        if (u != pr && alive[u] && 2 * sz[u] > sz[v]) {
          nxt = u;
          break;
        }
      }
      if (nxt == -1) {
        break;
      }
      pr = c;
      c = nxt;
    }
    if (parent_c != -1) {
#ifdef GRACIE
      res.parent_centroid[c] = parent_c;
#endif
    }
    res.push_back(c);
    alive[c] = false;
    for (int eid : g.g[c]) {
      auto& e = g.edges[eid];
      int u = e.from ^ e.to ^ c;
      if (alive[u]) {
        self(self, u, c);
      }
    }
  };
  for (int i = 0; i < n; i++) {
    if (alive[i]) {
      Build(Build, i, -1);
    }
  }
  return res;
}

