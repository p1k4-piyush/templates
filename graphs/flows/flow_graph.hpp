template <typename T>
class flow_graph {
 public:
  static constexpr T eps = (T) 1e-9;

  struct edge {
    int from;
    int to;
    T c;
    T f;
  };

  vector<vector<int>> g;
  vector<edge> edges;
  int n;
  int st;
  int fin;
  T flow;

  flow_graph(int _n, int _st, int _fin) : n(_n), st(_st), fin(_fin) {
    assert(0 <= st && st < n && 0 <= fin && fin < n && st != fin);
    g.resize(n);
    flow = 0;
  }

  void clear_flow() {
    for (edge &e : edges) {
      e.f = 0;
    }
    flow = 0;
  }
   
  int add(int from, int to, T forward_cap, T backward_cap) {
    assert(0 <= from && from < n && 0 <= to && to < n);
    int id = (int) edges.size();
    g[from].push_back(id);
    edges.push_back({from, to, forward_cap, 0});
    g[to].push_back(id + 1);
    edges.push_back({to, from, backward_cap, 0});
    return id;
  }

};

#ifdef GRACIE
template <typename T>
std::string graphviz(const flow_graph<T>& fg, const std::vector<int>& mincut = {}, bool simplify = true) {
    std::ostringstream out;
    out << "digraph G {\n";
    out << "  rankdir=LR;\n";
    out << "  node [shape=circle];\n";
    
    std::vector<bool> in_cut(fg.n, false);
    for(int v : mincut) in_cut[v] = true;

    for (int i = 0; i < fg.n; i++) {
        out << "  " << i << " [label=\"" << i << "\"";
        if (i == fg.st) out << ", style=filled, fillcolor=lightgreen";
        else if (i == fg.fin) out << ", style=filled, fillcolor=lightcoral";
        else if (in_cut[i]) out << ", style=filled, fillcolor=yellow";
        out << "];\n";
    }

    for (int id = 0; id < (int)fg.edges.size(); id += 2) {
        const auto& e = fg.edges[id];
        const auto& back = fg.edges[id ^ 1];
        if (simplify && e.c == 0 && back.c == 0) continue; 
        
        bool is_cut = in_cut[e.from] && !in_cut[e.to] && e.c > 0;
        out << "  " << e.from << " -> " << e.to << " [label=\"" << e.f << " / " << e.c << "\"";
        if (e.f > 0) out << ", color=blue, penwidth=2";
        if (is_cut) out << ", color=red, style=dashed, penwidth=2";
        out << "];\n";
        
        if (!simplify || back.c > 0) {
            out << "  " << back.from << " -> " << back.to << " [label=\"" << back.f << " / " << back.c << "\", style=dotted];\n";
        }
    }
    out << "}\n";
    return out.str();
}
#endif
