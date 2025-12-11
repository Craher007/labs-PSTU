
#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    int n; float x, nsum, fact = 1;
    cin >> n >> x;
    nsum = 1 + x;
    for (int i = 2; i < n; i++) {
        fact *= i;
        nsum +=( pow(x, i) / fact);
    }
    cout << nsum;
}
