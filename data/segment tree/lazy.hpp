template <typename Info, typename Tag>
class LazySegmentTree {
public:
    int n;
    vector<Info> infos;
    vector<Tag> tags;
    seg_tree::in_order_layout layout;

    void Apply(seg_tree::point a, const Tag& t)
    {
        auto [l, r] = layout.get_node_bounds(a);
        if (!t.ApplyTo(infos[a], l, r)) {
            assert(a < n);
            DowndateNode(a);
            Apply(a.c(0), t);
            Apply(a.c(1), t);
            UpdateNode(a);
            return;
        }
        if (a < n) {
            t.ApplyTo(tags[a]);
        }
    }

    void DowndateNode(seg_tree::point a)
    {
        if (!tags[a].Empty()) {
            Apply(a.c(0), tags[a]);
            Apply(a.c(1), tags[a]);
            tags[a] = Tag();
        }
    }

    void UpdateNode(seg_tree::point a)
    {
        infos[a] = infos[a.c(0)].Unite(infos[a.c(1)]);
    }

    LazySegmentTree()
        : LazySegmentTree(0)
    {
    }
    LazySegmentTree(int n_)
        : LazySegmentTree(vector<Info>(n_))
    {
    }
    LazySegmentTree(const vector<Info>& a)
        : n(int(a.size()))
    {
        infos.resize(2 * n);
        tags.resize(n);
        layout = seg_tree::in_order_layout(n);
        for (int i = 0; i < n; i++) {
            infos[layout.get_point(i)] = a[i];
        }
        for (int i = n - 1; i >= 1; i--) {
            UpdateNode(seg_tree::point(i));
        }
    }

    void Modify(int l, int r, const Tag& t)
    {
        auto rng = layout.get_range(l, r);
        rng.for_parents_down([&](seg_tree::point a) { DowndateNode(a); });
        rng.for_each([&](seg_tree::point a) { Apply(a, t); });
        rng.for_parents_up([&](seg_tree::point a) { UpdateNode(a); });
    }

    void Set(int p, const Info& v)
    {
        auto pt = layout.get_point(p);
        pt.for_parents_down([&](seg_tree::point a) { DowndateNode(a); });
        infos[pt] = v;
        pt.for_parents_up([&](seg_tree::point a) { UpdateNode(a); });
    }

    Info Query(int l, int r)
    {
        auto rng = layout.get_range(l, r);
        rng.for_parents_down([&](seg_tree::point a) { DowndateNode(a); });
        Info res;
        rng.for_each_l_to_r([&](seg_tree::point a) { res = res.Unite(infos[a]); });
        return res;
    }

    Info Get(int p)
    {
        auto pt = layout.get_point(p);
        pt.for_parents_down([&](seg_tree::point a) { DowndateNode(a); });
        return infos[pt];
    }

    template <typename F>
    int MaxRight(int l, F f)
    {
        auto rng = layout.get_range(l, n);
        rng.for_parents_down([&](seg_tree::point a) { DowndateNode(a); });
        int res = n;
        Info sum;
        rng.for_each_l_to_r(
            [&](seg_tree::point a) {
                if (res != n) {
                    return;
                }
                auto new_sum = sum.Unite(infos[a]);
                if (f(new_sum)) {
                    sum = new_sum;
                    return;
                }
                while (a < n) {
                    DowndateNode(a);
                    new_sum = sum.Unite(infos[a.c(0)]);
                    if (f(new_sum)) {
                        sum = new_sum;
                        a = a.c(1);
                    } else {
                        a = a.c(0);
                    }
                }
                res = layout.get_node_bounds(a)[0];
            });
        return res;
    }

    template <typename F>
    int MinLeft(int r, F f)
    {
        auto rng = layout.get_range(0, r);
        rng.for_parents_down([&](seg_tree::point a) { DowndateNode(a); });
        int res = 0;
        Info sum;
        rng.for_each_r_to_l(
            [&](seg_tree::point a) {
                if (res != 0) {
                    return;
                }
                auto new_sum = infos[a].Unite(sum);
                if (f(new_sum)) {
                    sum = new_sum;
                    return;
                }
                while (a < n) {
                    DowndateNode(a);
                    new_sum = infos[a.c(1)].Unite(sum);
                    if (f(new_sum)) {
                        sum = new_sum;
                        a = a.c(0);
                    } else {
                        a = a.c(1);
                    }
                }
                res = layout.get_node_bounds(a)[1];
            });
        return res;
    }

#ifdef GRACIE
    std::string graphviz() const {
        std::ostringstream os;
        os << "digraph G {\n";
        os << "  node [shape=plaintext, fontname=\"Courier\"];\n";
        
        auto encode_html = [](std::string s) {
            std::string res;
            for (char c : s) {
                if (c == '<') res += "&lt;";
                else if (c == '>') res += "&gt;";
                else if (c == '&') res += "&amp;";
                else if (c == '"') res += "&quot;";
                else if (c == '\'') res += "&#39;";
                else if (c == '\n') res += "<br/>";
                else res += c;
            }
            return res;
        };

        auto get_string = [](auto const& x) {
            if constexpr (requires { x.print(); }) {
                std::ostringstream ss; ss << x.print(); return ss.str();
            } else if constexpr (requires { x.to_string(); }) {
                std::ostringstream ss; ss << x.to_string(); return ss.str();
            } else if constexpr (requires { std::declval<std::ostream&>() << x; }) {
                std::ostringstream ss; ss << x; return ss.str();
            } else {
                std::ostringstream ss;
                _dbglib::dbg_impl(ss, x, false);
                return ss.str();
            }
        };

        for (int i = 1; i < 2 * n; i++) {
            auto bounds = layout.get_node_bounds(seg_tree::point(i));
            os << "  " << i << " [label=<\n";
            os << "    <table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"4\">\n";
            os << "      <tr><td bgcolor=\"lightgrey\">[" << bounds[0] << ", " << bounds[1] << ")</td></tr>\n";
            
            os << "      <tr><td>" << encode_html(get_string(infos[i])) << "</td></tr>\n";
            
            if (i < n) {
                os << "      <tr><td bgcolor=\"#ffffd5\">" << encode_html(get_string(tags[i])) << "</td></tr>\n";
            }
            
            os << "    </table>\n";
            os << "  >];\n";
        }
        
        for (int i = 1; i < n; i++) {
            os << "  " << i << " -> " << 2 * i << ";\n";
            os << "  " << i << " -> " << 2 * i + 1 << ";\n";
        }
        os << "}\n";
        return os.str();
    }
#endif
};
