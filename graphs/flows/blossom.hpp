
using namespace std;

#ifdef GRACIE
template <typename T>
struct blossom_result : public vector<int> {
    const undigraph<T>* g_ptr;

    std::string graphviz() const {
        std::ostringstream os;
        os << "graph G {\n"; 
        os << "  node [shape=circle, fontname=\"Courier\"];\n";
        
        const undigraph<T>& g = *g_ptr;
        for (int i = 0; i < g.n; ++i) {
            os << "  " << i << " [label=\"" << i << "\"];\n";
        }

        for (int i = 0; i < (int)g.edges.size(); i += 2) {
            const auto& e = g.edges[i];
            os << "  " << e.from << " -- " << e.to;
            if ((*this)[e.from] == e.to) { 
                os << " [color=\"red\", penwidth=3.5]";
            } else {
                os << " [color=\"gray80\"]";
            }
            os << ";\n";
        }
        os << "}\n";
        return os.str();
    }
};
#else
template <typename T>
using blossom_result = vector<int>;
#endif

template <typename T>
blossom_result<T> find_max_unweighted_matching(const undigraph<T>& g) {
  blossom_result<T> mate;
  mate.assign(g.n, -1);
#ifdef GRACIE
  mate.g_ptr = &g;
#endif

  vector<int> label(g.n);
  vector<int> parent(g.n);
  vector<int> orig(g.n);
  queue<int> q;
  vector<int> aux(g.n, -1);
  int aux_time = -1;
  auto lca = [&](int x, int y) {
    aux_time++;
    while (true) {
      if (x != -1) {
        if (aux[x] == aux_time) {
          return x;
        }
        aux[x] = aux_time;
        if (mate[x] == -1) {
          x = -1;
        } else {
          x = orig[parent[mate[x]]];
        }
      }
      swap(x, y);
    }
  };
  auto blossom = [&](int v, int w, int a) {
    while (orig[v] != a) {
      parent[v] = w;
      w = mate[v];
      if (label[w] == 1) {
        label[w] = 0;
        q.push(w);
      }
      orig[v] = orig[w] = a;
      v = parent[w];
    }
  };
  auto augment = [&](int v) {
    while (v != -1) {
      int pv = parent[v];
      int nv = mate[pv];
      mate[v] = pv;
      mate[pv] = v;
      v = nv;
    }
  };
  auto bfs = [&](int root) {
    fill(label.begin(), label.end(), -1);
    iota(orig.begin(), orig.end(), 0);
    while (!q.empty()) {
      q.pop();
    }
    q.push(root);
    label[root] = 0;
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int id : g.g[v]) {
        auto &e = g.edges[id];
        int x = e.from ^ e.to ^ v;
        if (label[x] == -1) {
          label[x] = 1;
          parent[x] = v;
          if (mate[x] == -1) {
            augment(x);
            return true;
          }
          label[mate[x]] = 0;
          q.push(mate[x]);
          continue;
        }
        if (label[x] == 0 && orig[v] != orig[x]) {
          int a = lca(orig[v], orig[x]);
          blossom(x, v, a);
          blossom(v, x, a);
        }
      }
    }
    return false;
  };
  auto greedy = [&]() {
    vector<int> order(g.n);
    iota(order.begin(), order.end(), 0);
    shuffle(order.begin(), order.end(), mt19937(787788));
    for (int i : order) {
      if (mate[i] == -1) {
        for (int id : g.g[i]) {
          auto &e = g.edges[id];
          int to = e.from ^ e.to ^ i;
          if (i != to && mate[to] == -1) {
            mate[i] = to;
            mate[to] = i;
            break;
          }
        }
      }
    }
  };
  greedy();
  for (int i = 0; i < g.n; i++) {
    if (mate[i] == -1) {
      bfs(i);
    }
  }
  return mate;
}
