template <typename T>
std::vector<int> centroid_decomposition(const forest<T>& g) {
  int n = g.n;
  vector<bool> alive(n, true);
  std::vector<int> res;
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

#ifdef GRACIE
template <typename T>
std::string graphviz_centroid(const forest<T>& g, const std::vector<int>& res) {
  int n = g.n;
  vector<bool> alive(n, true);
  vector<int> parent_centroid(n, -1);
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
      if (nxt == -1) break;
      pr = c;
      c = nxt;
    }
    if (parent_c != -1) parent_centroid[c] = parent_c;
    alive[c] = false;
    for (int eid : g.g[c]) {
      auto& e = g.edges[eid];
      int u = e.from ^ e.to ^ c;
      if (alive[u]) self(self, u, c);
    }
  };
  for (int i = 0; i < n; i++) if (alive[i]) Build(Build, i, -1);

  std::ostringstream os;
  os << "digraph G {\n  layout=twopi;\n  node [shape=circle, fontsize=10];\n";
  for (int i = 0; i < g.n; i++) {
     os << "  " << i << " [label=\"" << i << "\"];\n";
  }
  for (int i = 0; i < g.n; i++) {
     if (parent_centroid[i] != -1) {
         os << "  " << parent_centroid[i] << " -> " << i << " [color=red, penwidth=2.0];\n";
     }
  }
  for (auto const& e : g.edges) {
     os << "  " << e.from << " -> " << e.to << " [style=dotted, color=gray, dir=none, constraint=false];\n";
  }
  os << "}\n";
  return os.str();
}
#endif

