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

// clang-format off
namespace _dbglib {
enum {BLACK = 0, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, BRIGHT = 8, BG = 16, RESET = 24};

static constexpr char const* escape_codes[]{
	"\x1b[30m",   "\x1b[31m",   "\x1b[32m",   "\x1b[33m",   "\x1b[34m",   "\x1b[35m",   "\x1b[36m",
	"\x1b[37m",   "\x1b[30;1m", "\x1b[31;1m", "\x1b[32;1m", "\x1b[33;1m", "\x1b[34;1m", "\x1b[35;1m",
	"\x1b[36;1m", "\x1b[37;1m", "\x1b[40;1m", "\x1b[41;1m", "\x1b[42;1m", "\x1b[43;1m", "\x1b[44;1m",
	"\x1b[45;1m", "\x1b[46;1m", "\x1b[47;1m", "\x1b[0m",
};

constexpr auto ansi_escape_code(int style = RESET) { return escape_codes[style]; }
#define S(...) (styled ? ansi_escape_code(__VA_ARGS__) : "")
// clang-format on

template <class T>
constexpr bool is_string = std::is_same_v<T, std::string> or std::is_same_v<T, std::string_view> or std::is_same_v<std::decay_t<T>, char*> or std::is_same_v<std::decay_t<T>, const char*>;

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
        os << x;
    }
}

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
             dbg_pretty_impl(os, xs, styled),
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
    dbg_pretty_impl(os, x, styled),
        ((os << ' ' << S(WHITE | BG) << S(BLACK) << '|' << S() << ' ', dbg_pretty_impl(os, xs, styled)), ...),
        os << '\n';
}
#undef S
} // namespace _dbglib

#define dbg(...) _dbglib::dbg_repr(#__VA_ARGS__, std::source_location::current(), 1 __VA_OPT__(, ) __VA_ARGS__)
#define dbgl(...) _dbglib::dbg_repr_line(#__VA_ARGS__, std::source_location::current(), 1 __VA_OPT__(, ) __VA_ARGS__)
