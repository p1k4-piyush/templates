#pragma once

#include <bit>
#include <concepts>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <ranges>
#include <source_location>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <variant>

#include <cstdlib>
#include <fstream>
#include <map>
#include <mutex>
#include <sstream>

namespace _dbglib {
enum { BLACK = 0,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    BRIGHT = 8,
    BG = 16,
    RESET = 24 };

static constexpr char const* escape_codes[] {
    "\x1b[30m",
    "\x1b[31m",
    "\x1b[32m",
    "\x1b[33m",
    "\x1b[34m",
    "\x1b[35m",
    "\x1b[36m",
    "\x1b[37m",
    "\x1b[30;1m",
    "\x1b[31;1m",
    "\x1b[32;1m",
    "\x1b[33;1m",
    "\x1b[34;1m",
    "\x1b[35;1m",
    "\x1b[36;1m",
    "\x1b[37;1m",
    "\x1b[40;1m",
    "\x1b[41;1m",
    "\x1b[42;1m",
    "\x1b[43;1m",
    "\x1b[44;1m",
    "\x1b[45;1m",
    "\x1b[46;1m",
    "\x1b[47;1m",
    "\x1b[0m",
};

constexpr auto ansi_escape_code(int style = RESET) { return escape_codes[style]; }
#define S(...) (styled ? ansi_escape_code(__VA_ARGS__) : "")

template <class T>
constexpr bool is_string = std::is_same_v<T, std::string> or std::is_same_v<T, std::string_view> or std::is_same_v<std::decay_t<T>, char*> or std::is_same_v<std::decay_t<T>, const char*>;

// Fallback logic for user-defined generic structures like vectors in Segment Trees
template <typename T>
concept _GracieIterable = requires(T x) { std::begin(x); std::end(x); } && !std::is_same_v<T, std::string>;

template <typename T>
concept has_gracie_ostream = requires(std::ostream& os, const T& x) { os << x; };

template <typename T>
concept has_gracie_print = requires(const T& x) { x.print(); };

template <typename T>
concept has_gracie_to_string = requires(const T& x) { x.to_string(); };

inline void gracie_safe_print(std::ostream& os, const auto& x, int depth = 0) {
    using T = std::remove_cvref_t<decltype(x)>;
    if constexpr (has_gracie_ostream<T>) {
        os << x;
    } else if constexpr (has_gracie_print<T>) {
        os << x.print();
    } else if constexpr (has_gracie_to_string<T>) {
        os << x.to_string();
    } else if constexpr (_GracieIterable<T>) {
        os << "{";
        bool first = true;
        for (const auto& i : x) {
            if (!first) os << ", ";
            first = false;
            gracie_safe_print(os, i, depth + 1);
        }
        os << "}";
    } else if constexpr (requires { std::tuple_size<T>{}; }) {
        os << "(";
        std::apply([&](auto const&... xs) {
            int f = 0;
            ((os << (f++ ? ", " : ""), gracie_safe_print(os, xs, depth + 1)), ...);
        }, x);
        os << ")";
    } else {
        os << "[Struct]";
    }
}

void dbg_impl(std::ostream& os, auto const& x, bool styled = true, int d = 0)
{
    using T = std::remove_cvref_t<decltype(x)>;
    auto f = 0, brack = d % 6 + 1;

    if constexpr (std::is_same_v<T, char>)
        os << '\'' << S(GREEN) << x << S() << '\'';
    else if constexpr (std::is_same_v<T, bool>)
        os << S(x ? GREEN : BLUE) << "FT"[x] << S();
    else if constexpr (std::signed_integral<T>)
        os << x;
    else if constexpr (std::unsigned_integral<T>) {
        os << x << " = ";
        for (auto i = ((std::bit_width(x) - 1) + 8) / 8 * 8; i-- > 0;)
            os << S(i >> 3 & 1 ? GREEN : BLUE) << (x >> i & 1 ? '1' : 'o') << S();
    } else if constexpr (is_string<T>) {
        os << '"' << S(GREEN) << x << S() << '"';
    } else if constexpr (std::ranges::input_range<T>) {
        os << S(brack) << '[' << S();
        for (auto const& i : x)
            os << (f++ ? ", " : ""), dbg_impl(os, i, styled, d + 1);
        os << S(brack) << ']' << S();
    } else if constexpr (requires { std::declval<T>().pop(); }) {
        os << S(brack) << '{' << S();
        for (auto v = x; std::size(v); v.pop()) {
            os << (f++ ? ", " : "");
            if constexpr (requires { v.top(); })
                dbg_impl(os, v.top(), styled, d + 1);
            else
                dbg_impl(os, v.front(), styled, d + 1);
        }
        os << S(brack) << '}' << S();
    } else if constexpr (requires { std::tuple_size<T> {}; }) {
        os << S(brack) << '(' << S();
        std::apply([&](auto const&... xs) { ((os << (f++ ? ", " : ""), dbg_impl(os, xs, styled, d + 1)), ...); }, x);
        os << S(brack) << ')' << S();
    } else if constexpr (requires { std::variant_size<T> {}; }) {
        std::visit([&](auto const& v) { dbg_impl(os, v, styled, d); }, x);
    } else {
        os << x;
    }
}

template <class T>
constexpr bool is_homogeneous_container = (std::ranges::input_range<T> or requires { std::declval<T>().pop(); }) and requires { typename T::value_type; };

template <class T>
constexpr bool is_messy = !std::signed_integral<T> and !std::floating_point<T> and !std::is_same_v<T, bool>;

constexpr bool deserves_newline(auto const& x)
{
    constexpr size_t THRESHOLD = 5;
    using T = std::remove_cvref_t<decltype(x)>;

    if constexpr (is_string<T>) {
        return std::size(std::string_view { x }) > 20 * THRESHOLD;
    } else if constexpr (std::is_array_v<T>) {
        return std::rank_v<T> > 1 or std::size(x) > THRESHOLD;
    } else if constexpr (is_homogeneous_container<T>) {
        return is_messy<typename T::value_type> or std::size(x) > THRESHOLD;
    } else if constexpr (requires { std::tuple_size<T> {}; }) {
        return std::apply([&](auto const&... xs) { return (is_messy<std::remove_cvref_t<decltype(xs)>> or ...); }, x) or std::tuple_size_v<T> > THRESHOLD;
    } else if constexpr (requires { std::variant_size<T> {}; }) {
        return std::visit([&](auto const& v) { return deserves_newline(v); }, x);
    } else {
        return false;
    }
}

void dbg_pretty_impl(std::ostream& os, auto const& x, bool styled = true, int d = 0)
{
    using T = std::remove_cvref_t<decltype(x)>;
    auto brack = d % 6 + 1;

    auto indent = [&os](int t) {
        while (t--)
            os << "    ";
    };

    auto dbg_element = [&, f = 0](auto const& i = 0) mutable {
        constexpr auto SHOW_IDX_EVERY = 10, BREAK_AFTER = 30;
        auto deserves = deserves_newline(i), show_idx = f and f % SHOW_IDX_EVERY == 0;
        if (deserves or (f and f % BREAK_AFTER == 0))
            os << (f++ ? ",\n" : "\n"), indent(d + 1 + !deserves), show_idx = true;
        else
            os << (f++ ? ", " : "");
        if (show_idx)
            os << S(brack) << std::setw(4) << f - 1 << ": " << S();
        dbg_pretty_impl(os, i, styled, d + 1);
    };

    if constexpr (std::is_same_v<T, char>)
        os << '\'' << S(GREEN) << x << S() << '\'';
    else if constexpr (std::is_same_v<T, bool>)
        os << S(x ? GREEN : BLUE) << "FT"[x] << S();
    else if constexpr (std::signed_integral<T>)
        os /* << std::setw(x > 999999 ? 14 : x > 99 ? 6 : 2) */ << x;
    else if constexpr (std::unsigned_integral<T>) {
        os << x << " = ";
        for (auto i = ((std::bit_width(x) - 1) + 8) / 8 * 8; i-- > 0;)
            os << S((i >> 3 & 1 ? GREEN : BLUE) /* | (i & 1 ? 0 : BRIGHT) */) << (x >> i & 1 ? '1' : 'o') << S();
    } else if constexpr (is_string<T>) {
        os << '"' << S(GREEN) << x << S() << '"';
    } else if constexpr (std::ranges::input_range<T>) {
        os << S(brack) << '[' << S();
        for (auto const& i : x)
            dbg_element(i);
        os << S(brack) << ']' << S();
    } else if constexpr (requires { std::declval<T>().pop(); }) {
        os << S(brack) << '{' << S();
        for (auto v = x; std::size(v); v.pop()) {
            if constexpr (requires { v.top(); })
                dbg_element(v.top());
            else
                dbg_element(v.front());
        }
        os << S(brack) << '}' << S();
    } else if constexpr (requires { std::tuple_size<T> {}; }) {
        os << S(brack) << '(' << S();
        std::apply([&](auto const&... xs) { (dbg_element(xs), ...); }, x);
        os << S(brack) << ')' << S();
    } else if constexpr (requires { std::variant_size<T> {}; }) {
        std::visit([&](auto const& v) { dbg_pretty_impl(os, v, styled, d); }, x);
    } else {
        gracie_safe_print(os, x, d);
    }
}

// ---------- Graphviz helpers (paste here) ----------
static std::map<int, int> _dbg_graph_counters;
static std::mutex _dbg_graph_mutex;

inline std::string _dbg_make_png_name(int line, int count)
{
    std::ostringstream ss;
    ss << "out_" << line << '_' << count << ".png";
    return ss.str();
}
inline std::string _dbg_make_dot_name(int line, int count)
{
    std::ostringstream ss;
    ss << "out_" << line << '_' << count << ".dot";
    return ss.str();
}

inline std::string save_graph_and_open(int line, std::string const& gv)
{
    std::lock_guard<std::mutex> guard(_dbg_graph_mutex);
    int cnt = ++_dbg_graph_counters[line];
    auto dotname = _dbg_make_dot_name(line, cnt);
    auto pngname = _dbg_make_png_name(line, cnt);

    {
        std::ofstream ofs(dotname, std::ios::binary);
        ofs << gv;
    }

    std::string engine = "neato";
    if (auto pos = gv.find("layout="); pos != std::string::npos) {
        auto end = gv.find_first_of(" ;\"\n\r", pos + 7);
        engine = gv.substr(pos + 7, end - (pos + 7));
        if (!engine.empty() && engine[0] == '"') {
            engine = engine.substr(1, engine.length() - 2);
        }
    } else if (gv.find("digraph") != std::string::npos) {
        engine = "dot";
    }

    std::string cmd = engine + " -Tpng " + dotname + " -o " + pngname + "; open " + pngname + "";
    std::system(cmd.c_str());
    return pngname;
}


template <typename U>
void debug_print_with_graph(std::ostream& os, int line, U const& v, bool styled, int d = 0)
{
    if constexpr (requires { v.graphviz(); }) {
        using R = decltype(v.graphviz());
        std::string s;
        if constexpr (std::is_convertible_v<R, std::string>) {
            s = v.graphviz();
        } else if constexpr (std::is_convertible_v<R, std::string_view>) {
            s = std::string(v.graphviz());
        } else if constexpr (std::is_convertible_v<R, const char*>) {
            s = std::string(v.graphviz());
        } else {
            std::ostringstream tmp;
            tmp << v.graphviz();
            s = tmp.str();
        }
        auto png = save_graph_and_open(line, s);
        os << S(GREEN) << "<graphviz:" << png << ">" << S();
        return;
    }

    auto looks_like_dot = [&](std::string_view s) {
        if (s.empty())
            return false;
        // Strict boundary validation to strictly avoid blindly catching conversational "test digraph" titles!
        std::string h(s);
        std::transform(h.begin(), h.end(), h.begin(), [](unsigned char c) { return std::tolower(c); });
        
        // Strip leading whitespace
        size_t start = h.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return false;
        h = h.substr(start);
        
        if (s.find('{') == std::string::npos || s.find('}') == std::string::npos)
            return false;
            
        if (h.starts_with("digraph") || h.starts_with("strict digraph") || h.starts_with("graph ") || h.starts_with("strict graph"))
            return true;
            
        return false;
    };

    if constexpr (requires { std::declval<std::ostream&>() << std::declval<U const&>(); }) {
        std::ostringstream ss;
        ss << v;
        auto s = ss.str();

        if (looks_like_dot(s)) {
            auto png = save_graph_and_open(line, s);
            os << S(GREEN) << "<graphviz:" << png << ">" << S();
        } else {
            dbg_pretty_impl(os, v, styled, d);
        }
        return;
    }

    dbg_pretty_impl(os, v, styled, d);
}
// ---------- end Graphviz helpers ----------

constexpr int scan_expr(char const* exprs)
{
    auto i = 0;
    for (auto d = 0, q = 0; exprs[i] and (d or q or exprs[i] != ','); ++i) {
        auto c = exprs[i];
        if (q and c == '\\')
            i++;
        else if (q and c == q)
            q = 0;
        else if (!q and (c == '\'' or c == '"'))
            q = c;
        else if (c == '[' or c == '{' or c == '(')
            d++;
        else if (c == ']' or c == '}' or c == ')')
            d--;
        else if (c == '<' and exprs[i + 1] == '=' and exprs[i + 2] == '>')
            i += 2;
        else if (c == '<' or c == '>') {
            if (exprs[i + 1] == '=' or exprs[i + 1] == c)
                i++;
            else
                d += '=' - c; // TODO: vector{1 > 2 and 3 > 1} can lead to negative d
        }
    }
    return i;
}

void dbg_repr(char const* repr, std::source_location loc, bool styled, auto const&... xs)
{
    auto& os = std::cerr;
    auto loc_info = [&] { os << S(WHITE | BG) << S(BLACK) << '[' << std::setw(3) << loc.line() << ']' << S(); };
    if constexpr (sizeof...(xs)) {
        auto pop_expr = [&] {
            auto i = scan_expr(repr);
            if (i < 0 or !repr[i])
                return std::string_view { repr };
            std::string_view expr(repr, size_t(i));
            repr += i;
            if (*repr == ',')
                repr++;
            while (*repr == ' ')
                repr++;
            return expr;
        };
        ((loc_info(),
             os << ' ' << S(YELLOW) << pop_expr() << S() << " = ",
             // <-- use graph-aware print wrapper here:
             debug_print_with_graph(os, loc.line(), xs, styled),
             os << '\n'),
            ...);
    } else {
        loc_info(), os << '\n';
    }
}

void dbg_repr_line(char const* repr, std::source_location loc, bool styled, auto const& x, auto const&... xs)
{
    auto& os = std::cerr;
    os << S(WHITE | BG) << S(BLACK) << '[' << std::setw(3) << loc.line() << ']' << S();
    os << ' ' << S(YELLOW) << repr << S() << " = ";
    // first element via graph-aware helper
    debug_print_with_graph(os, loc.line(), x, styled);
    // remaining elements separated by '|'
    ((os << ' ' << S(WHITE | BG) << S(BLACK) << '|' << S() << ' ', debug_print_with_graph(os, loc.line(), xs, styled)), ...),
        os << '\n';
}
#undef S
} // namespace _dbglib

#define dbg(...) _dbglib::dbg_repr(#__VA_ARGS__, std::source_location::current(), 1 __VA_OPT__(, ) __VA_ARGS__)
#define dbgl(...) _dbglib::dbg_repr_line(#__VA_ARGS__, std::source_location::current(), 1 __VA_OPT__(, ) __VA_ARGS__)
