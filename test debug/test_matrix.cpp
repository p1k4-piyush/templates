#include "bits/stdc++.h"
using namespace std;
#include "../template/dbg.hpp"
#include "../maths-numeric/matrix.hpp"

int main() {
    int n = 10;
    matrix<int> a(n, vector<int>(n));
    matrix<int> b(n, vector<int>(n));
    mt19937 rng(1337);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            uniform_int_distribution<int> unif(1, 10);
            a[i][j] = unif(rng);
            b[i][j] = unif(rng);
        }
    }
    
    matrix<int> c = a * b;
    dbg("Matrix Multiply Scaling Test", c);
    
    assert((int)c.size() == n);
    return 0;
}
