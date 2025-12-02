
#include <iostream>
using namespace std;
int main()
{
    int n,su=0;
    cin >> n;
    for (int i = 1; i <= n; i++) {
        int t = 1;
        for (int j = i; j <= 2 * i; j++) {
            t *= j;
        }
        su += t;

    }
    cout << su;
}

