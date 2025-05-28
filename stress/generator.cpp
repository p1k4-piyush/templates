
//	https://github.com/p1k4-piyush/templates/

#include <bits/stdc++.h>
using namespace std;

typedef int64_t ll;
const ll        INF = ll(4e18) + 5;
const char      nl  = '\n';

const ll  e18 = ll(1e18);
const int e9  = int(1e9);
const int e5  = int(1e5);

#ifdef DEBUG
#include "/home/dbg/debug.cpp"
#else
#define dbg(...)
#endif

mt19937_64 rng((unsigned int)chrono::steady_clock::now().time_since_epoch().count());

void normal()
{
    uniform_int_distribution<int> tt(0, e5);

    int n = e5 + tt(rng);
    cout << n << nl;

    uniform_int_distribution<int> dist(1, e9);

    while (n--)
    {
        cout << dist(rng) << " ";
    }

    cout << nl;
    return;
}

void normal_n()
{
    uniform_int_distribution<int> tt(0, e5);

    int n = e5 + tt(rng);
    cout << n << nl;

    uniform_int_distribution<int> dist(1, n);

    while (n--)
    {
        cout << dist(rng) << " ";
    }

    cout << nl;
    return;
}

void single_n()
{
    uniform_int_distribution<ll> tt(1, e18);

    ll n = tt(rng);
    cout << n << nl;

    return;
}

void perm()
{
    uniform_int_distribution<int> tt(0, e5);

    int n = 100000 + tt(rng);
    cout << n << nl;

    vector<int> perm(n);

    iota(perm.begin(), perm.end(), 1);
    shuffle(perm.begin(), perm.end(), rng);

    for (auto i : perm)
    {
        cout << i << " ";
    }

    cout << nl;
    return;
}

void tree()
{
    uniform_int_distribution<int> tt(1, 5);
    int                           n = 5 + tt(rng);

    vector<int>                   prufer(n - 2);
    uniform_int_distribution<int> dist(1, n);

    for (int i = 0; i < n - 2; i++)
    {
        prufer[i] = dist(rng);
    }

    vector<int> degree(n + 1, 1);

    for (int x : prufer)
    {
        degree[x]++;
    }

    set<int> leaves;

    for (int i = 1; i <= n; i++)
    {
        if (degree[i] == 1)
            leaves.insert(i);
    }

    vector<pair<int, int>> edges;

    for (int x : prufer)
    {
        int leaf = *leaves.begin();
        leaves.erase(leaves.begin());

        edges.push_back({leaf, x});
        degree[x]--;

        if (degree[x] == 1)
        {
            leaves.insert(x);
        }
    }

    int u = *leaves.begin();
    leaves.erase(leaves.begin());

    int v = *leaves.begin();
    edges.push_back({u, v});

    uniform_int_distribution<int> b(0, 1);
    for (auto& edge : edges)
    {
        if (b(rng) == 1)
        {
            swap(edge.first, edge.second);
        }
    }

    shuffle(edges.begin(), edges.end(), rng);

    cout << n << nl;

    for (auto& edge : edges)
    {
        cout << edge.first << " " << edge.second << nl;
    }

    return;
}

void tree_basic()
{
    uniform_int_distribution<int> tt(0, e5);

    int n = e5 + tt(rng);
    cout << n << nl;

    vector<pair<int, int>> vec;

    uniform_int_distribution<int> b(0, 1);
    for (int i = 2; i <= n; i++)
    {
        int par = uniform_int_distribution<int>(1, i - 1)(rng);

        pair<int, int> edge = {par, i};

        if (b(rng))
        {
            swap(edge.first, edge.second);
        }

        vec.push_back(edge);
    }

    shuffle(vec.begin(), vec.end(), rng);

    for (auto i : vec)
    {
        cout << i.first << " " << i.second << nl;
    }

    return;
}

void custom()
{
    uniform_int_distribution<int> tt(0, e5);

    int n = e5 + tt(rng);
    cout << n << nl;

    vector<int> arr;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    cout << t << nl;

    for (int tt = 0; tt < t; tt++)
    {

        // normal();
        // normal_n();
        // single_n();
        // perm();
        // tree();
        // custom();
    }

    return 0;
}
