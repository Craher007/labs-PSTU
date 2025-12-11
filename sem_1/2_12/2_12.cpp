
#include <iostream>
using namespace std;
int main()
{
    int n, a;
    cin >> n;
    if (n == 0) {
        return 0;
    }
    cin >> a;
    int nmax = a, nmin = a;
    for (int i = 1; i < n; i++) {
        cin >> a;
        if (nmax < a) { nmax = a; }
        else if (nmin > a) { nmin = a; }
    }
    cout << nmax + nmin;
}

