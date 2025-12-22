#include <iostream>
using namespace std;
int main() {
    int n, nmax, nmin=0,a;
    cin >> n>>nmin;
    nmax = nmin;
    int* ptr_max = &nmax, * ptr_min = &nmin;
    for (int i = 1; i < n; i++) {
        cin >> a;
        if (a > *ptr_max) { *ptr_max = a; }
        else if (a < *ptr_min) { *ptr_min = a; }
    }
    cout << *ptr_max<<" "<<*ptr_min;
}