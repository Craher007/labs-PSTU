
#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    setlocale(LC_ALL, "Russian");
    int n;
    double s;
    bool flag = false;
    cin >> n >> s;
    for (float i = 1; i <= n; i++) {
        double a = sin(n + (i / n));
        if (abs(s-a) <0.000001) { flag = true; }
    }
    if (flag) { 
        cout << "Число " << s << " входит в промежуток";
    }
    else{ cout << "Число " << s << " не входит в промежуток"; }
}
