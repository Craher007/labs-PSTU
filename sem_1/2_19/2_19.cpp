

#include <iostream>
using namespace std;
int main()
{
    int n, nsum = 0;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        if (i % 3 == 0) { nsum -= i; }
        else { nsum += i; }
    }
    cout << nsum;
}
