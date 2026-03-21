#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

double sum_n(double x, int n, double term, double sum, int i) {
    if (i > n) { return sum; }
    if (i == 0) {
        term = 1.0;
        sum = 1.0;
        return sum_n(x, n, term, sum, 1);
    }
    term *= x / i;
    sum += term;
    return sum_n(x, n, term, sum, i + 1);
}


double sum_eps(double x, double eps, double term, double sum, int i) {
    if (i == 0) {
        term = 1.0;
        sum = 1.0;
        return sum_eps(x, eps, term, sum, 1);
    }
    term *= x / i;
    if (fabs(term) < eps) { return sum; }
    sum += term;
    return sum_eps(x, eps, term, sum, i + 1);
}

int main() {
    double a = 1.0, b = 2.0;
    int k = 10;
    double h = (b - a) / k;
    int n = 15;
    double eps = 1e-4;

    cout << fixed << setprecision(6);
    cout << "x\t\tSN (n=" << n << ")\tSE (eps=" << eps << ")" << endl;
    cout << "------------------------------------------------" << endl;

    for (double x = a; x <= b + 1e-9; x += h) {
        double SN = sum_n(x, n, 0, 0, 0);
        double SE = sum_eps(x, eps, 0, 0, 0);
        cout << x << "\t" << SN << "\t" << SE << endl;
    }
    return 0;
}