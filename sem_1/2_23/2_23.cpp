
#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");
    int n, number=0;
    double nmax = -1;
    bool flag = false;
    cin >> n;
    for (float i = 1; i <= n; i++) {
        double a = sin(n + (i / n));
        if (nmax < a) {
            nmax = a;
        }
    }
    for (float i = 1; i <= n; i++) {
        double a = sin(n + (i / n));
        if (abs(nmax - a) < 0.000001) { number += 1; }
    }
    cout <<endl<< number<<nmax;
}
