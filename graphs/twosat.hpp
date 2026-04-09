class twosat {
 public:
  digraph<int> g;
  int n;

  twosat(int _n) : g(digraph<int>(2 * _n)), n(_n) {
  }

  // (v[x] == value_x)
  inline void add(int x, int value_x) {
    assert(0 <= x && x < n);
    assert(0 <= value_x && value_x <= 1);
    g.add(2 * x + (value_x ^ 1), 2 * x + value_x);
  }
  
  // (v[x] == value_x || v[y] == value_y)
  inline void add(int x, int value_x, int y, int value_y) {
    assert(0 <= x && x < n && 0 <= y && y < n);
    assert(0 <= value_x && value_x <= 1 && 0 <= value_y && value_y <= 1);
    g.add(2 * x + (value_x ^ 1), 2 * y + value_y);
    g.add(2 * y + (value_y ^ 1), 2 * x + value_x);
  }

#ifdef GRACIE
  std::string graphviz() const {
    std::ostringstream out;
    out << "digraph G {\n";
    out << "  node [shape=circle, margin=0.03, fontsize=10];\n";
    for (int i = 0; i < n; i++) {
        out << "  " << 2 * i << " [label=\"!" << i << "\"];\n";
        out << "  " << 2 * i + 1 << " [label=\"" << i << "\"];\n";
    }
    for (int i = 0; i < 2 * n; i++) {
        for (int id : g.g[i]) {
            int to = g.edges[id].to;
            out << "  " << i << " -> " << to << ";\n";
        }
    }
    out << "}\n";
    return out.str();
  }
#endif

#ifdef GRACIE
  struct twosat_result : public vector<int> {
      const twosat& ts;
      twosat_result(const vector<int>& res, const twosat& _ts) : vector<int>(res), ts(_ts) {}
      
      std::string graphviz() const {
          std::ostringstream out;
          out << "digraph G {\n";
          out << "  node [shape=circle, margin=0.03, fontsize=10];\n";
          for (int i = 0; i < ts.n; i++) {
              int val = (*this)[i];
              out << "  " << 2 * i << " [label=\"!" << i << "\", style=filled, fillcolor=" << (val == 0 ? "lightgreen" : "lightcoral") << "];\n";
              out << "  " << 2 * i + 1 << " [label=\"" << i << "\", style=filled, fillcolor=" << (val == 1 ? "lightgreen" : "lightcoral") << "];\n";
          }
          for (int i = 0; i < 2 * ts.n; i++) {
              for (int id : ts.g.g[i]) {
                  int to = ts.g.edges[id].to;
                  out << "  " << i << " -> " << to << ";\n";
              }
          }
          out << "}\n";
          return out.str();
      }
      friend std::ostream& operator<<(std::ostream& os, const twosat_result& t) {
          return os << "[TwoSat Solved=" << !t.empty() << "]";
      }
  };
#endif

#ifdef GRACIE
  inline twosat_result solve() {
#else
  inline vector<int> solve() {
#endif
    int cnt;
    vector<int> c = find_scc(g, cnt);
    vector<int> res(n);
    for (int i = 0; i < n; i++) {
      if (c[2 * i] == c[2 * i + 1]) {
#ifdef GRACIE
        return twosat_result(vector<int>(), *this);
#else
        return vector<int>();
#endif
      }
      res[i] = (c[2 * i] < c[2 * i + 1]);
    }
#ifdef GRACIE
    return twosat_result(res, *this);
#else
    return res;
#endif
  }
};
