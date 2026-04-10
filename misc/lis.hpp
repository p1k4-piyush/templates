

/* OLD:
template <typename T>
int lis(const vector<T>& a)
{
    vector<T> u;
    for (const T& x : a) {
        auto it = lower_bound(u.begin(), u.end(), x);
        if (it == u.end()) {
            u.push_back(x);
        } else {
            *it = x;
        };
    };
    return (int)u.size();
};
*/
template <typename T>
vector<T> lis(const vector<T>& a)
{
    vector<T> u;
    vector<int> p(a.size(), -1);
    vector<int> u_idx;
    for (int i = 0; i < (int)a.size(); i++) {
        auto it = lower_bound(u.begin(), u.end(), a[i]);
        int pos = distance(u.begin(), it);
        if (it == u.end()) {
            u.push_back(a[i]);
            u_idx.push_back(i);
        } else {
            *it = a[i];
            u_idx[pos] = i;
        }
        if (pos > 0) {
            p[i] = u_idx[pos - 1];
        }
    }
    vector<T> res;
    if (u_idx.empty())
        return res;
    int curr = u_idx.back();
    while (curr != -1) {
        res.push_back(a[curr]);
        curr = p[curr];
    }
    reverse(res.begin(), res.end());
    return res;
}
