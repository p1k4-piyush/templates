
//	https://github.com/p1k4-piyush/templates/

#include "bits/stdc++.h"
#include <string>

using namespace std;

#ifdef PIKA
#include "/Users/piyushkeshan/Documents/cpp_template_library/template/dbg.cpp"
#else
#define dbg(...)
#endif

typedef int64_t ll;
// #defint int ll

const ll inf = ll(4e18) + 5;
const char nl = '\n';

mt19937_64 rng((unsigned int)chrono::steady_clock::now().time_since_epoch().count());

void normal()
{
    uniform_int_distribution<int> tt(0, 2e5);
    uniform_int_distribution<int> dist(1, 1e9);

    string s;
    int count = 0;

    while (count < 2e5) {
        int n = tt(rng);
        s += (to_string(n) + nl);

        while (n--) {
            s += (to_string(dist(rng)) + " ");
        }

        s += nl;
        count += n;
    }
    cout << count << nl << s;
    return;
}

void normal_n()
{
    uniform_int_distribution<int> tt(0, 2e5);

    string s;
    int count = 0;

    while (count < 2e5) {
        int n = tt(rng);
        s += (to_string(n) + nl);

        uniform_int_distribution<int> dist(1, n);

        while (n--) {
            s += (to_string(dist(rng)) + " ");
        }

        s += nl;
        count += n;
    }
    cout << count << nl << s;
    return;
}

void single_n()
{
    uniform_int_distribution<ll> tt(1, ll(1e18));
    cout << "1000" << nl;
    for (int i = 0; i < 1000; i++) {
        cout << tt(rng) << nl;
    }

    return;
}

void perm()
{
    uniform_int_distribution<int> tt(0, 2e5);

    string s;
    int count = 0;

    while (count < 2e5) {
        int n = tt(rng);
        s += (to_string(n) + nl);

        vector<int> perm(n);

        iota(perm.begin(), perm.end(), 1);
        shuffle(perm.begin(), perm.end(), rng);

        for (auto i : perm) {
            s += (to_string(i) + " ");
        }

        s += nl;
        count += n;
    }
    cout << count << nl << s;
    return;
}

void tree()
{
    uniform_int_distribution<int> tt(1, 2e5);
    int n = tt(rng);

    vector<int> prufer(n - 2);
    uniform_int_distribution<int> dist(1, n);

    for (int i = 0; i < n - 2; i++) {
        prufer[i] = dist(rng);
    }

    vector<int> degree(n + 1, 1);

    for (int x : prufer) {
        degree[x]++;
    }

    set<int> leaves;

    for (int i = 1; i <= n; i++) {
        if (degree[i] == 1)
            leaves.insert(i);
    }

    vector<pair<int, int>> edges;

    for (int x : prufer) {
        int leaf = *leaves.begin();
        leaves.erase(leaves.begin());

        edges.push_back({ leaf, x });
        degree[x]--;

        if (degree[x] == 1) {
            leaves.insert(x);
        }
    }

    int u = *leaves.begin();
    leaves.erase(leaves.begin());

    int v = *leaves.begin();
    edges.push_back({ u, v });

    uniform_int_distribution<int> b(0, 1);
    for (auto& edge : edges) {
        if (b(rng) == 1) {
            swap(edge.first, edge.second);
        }
    }

    shuffle(edges.begin(), edges.end(), rng);

    cout << n << nl;

    for (auto& edge : edges) {
        cout << edge.first << " " << edge.second << nl;
    }

    return;
}

void tree_basic()
{
    uniform_int_distribution<int> tt(0, 2e5);

    int n = tt(rng);
    cout << n << nl;

    vector<pair<int, int>> vec;

    uniform_int_distribution<int> b(0, 1);

    for (int i = 2; i <= n; i++) {
        int par = uniform_int_distribution<int>(1, i - 1)(rng);

        pair<int, int> edge = { par, i };

        if (b(rng)) {
            swap(edge.first, edge.second);
        }

        vec.push_back(edge);
    }

    shuffle(vec.begin(), vec.end(), rng);

    for (auto i : vec) {
        cout << i.first << " " << i.second << nl;
    }

    return;
}

void custom()
{
    uniform_int_distribution<int> tt(0, 2e5);

    int n = tt(rng);
    cout << n << nl;

    vector<int> arr;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // normal();
    // normal_n();
    // single_n();
    // perm();
    // tree();
    // custom();

    return 0;
}
