
#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");
    int n,number;
    double nmax=-1;
    bool flag = false;
    cin >> n;
    for (float i = 1; i <= n; i++) {
        double a = sin(n + (i / n));
        if (nmax < a) {
            nmax = a;
            number = i;
        }
    }
    cout <<"Максимальное число: " << nmax << endl << "Номер: " << number;
}
