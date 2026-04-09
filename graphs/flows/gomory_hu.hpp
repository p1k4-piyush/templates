#ifdef GRACIE
template <typename T>
struct gomory_hu_result : public forest<T> {
  gomory_hu_result(const forest<T>& f) : forest<T>(f) {}
  std::string graphviz() const {
    std::ostringstream out;
    out << "graph G {\n";
    out << "  node [shape=circle, style=filled, fillcolor=lightblue];\n";
    for (int i = 0; i < this->n; i++) out << "  " << i << " [label=\"" << i << "\"];\n";
    for (const auto& e : this->edges) {
      if (e.from < e.to) {
        out << "  " << e.from << " -- " << e.to;
        std::ostringstream opts;
        if (e.cost != 1) opts << "label=\"" << e.cost << "\"";
        if (opts.str().length() > 0) opts << ", ";
        opts << "penwidth=2";
        out << " [" << opts.str() << "];\n";
      }
    }
    out << "}\n";
    return out.str();
  }
  friend std::ostream& operator<<(std::ostream& os, const gomory_hu_result& g) {
      return os << "[GomoryHu Tree Nodes=" << g.n << "]";
  }
};
template <typename T>
gomory_hu_result<T> gomory_hu(const undigraph<T>& g) {
#else
template <typename T>
forest<T> gomory_hu(const undigraph<T>& g) {
#endif
  int n = g.n;
  flow_graph<T> fg(n, 0, 1);
  for (auto& e : g.edges) {
    fg.add(e.from, e.to, e.cost, e.cost);
  }
  forest<T> ret(n);
  vector<int> pr(n, 0);
  for (int i = 1; i < n; i++) {
    fg.clear_flow();
    fg.st = i;
    fg.fin = pr[i];
    dinic<T> din(fg);
    T flow = din.max_flow();
    vector<bool> cut = din.min_cut();
    for (int j = i + 1; j < n; j++) {
      if (cut[j] == cut[i] && pr[j] == pr[i]) {
        pr[j] = i;
      }
    }
    ret.add(i, pr[i], flow);
  }
#ifdef GRACIE
  return gomory_hu_result<T>(ret);
#else
  return ret;
#endif
  // can be optimized by compressing components
}
