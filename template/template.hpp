#include "bits/stdc++.h"
using namespace std;

#ifdef GRACIE
#include "/Users/piyushkeshan/Documents/cpp_template_library/template/dbg.cpp"
#else
#define dbg(...)
#endif

typedef int64_t ll;
// #define int ll

const ll inf = ll(4e18) + 5;
const char nl = '\n';

void mess_it_up()
{
    int n;
    cin >> n;
    vector<int> arr(n, 0);

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    cout << nl;
    return;
}

signed main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef GRACIE
    auto i_knew_it_i_knew_you = std::chrono::high_resolution_clock::now();
#endif
    int let_it_happen = 1;
    cin >> let_it_happen;
    for (int cool = 1; cool <= let_it_happen; cool++) {
        dbg(cool);
        mess_it_up();
    }
    dbg(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - i_knew_it_i_knew_you).count() * 1e-9);
    return 0;
}
