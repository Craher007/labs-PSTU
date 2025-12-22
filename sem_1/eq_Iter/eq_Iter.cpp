
#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    float x, xPrev; // [2;4]
    double eps = 0.000001;
    double lambda = -0.1;
    x = 4;
    xPrev = 0;
    while (abs(x - xPrev) > eps) {
        xPrev = x;
        x = xPrev+ lambda * (3 * x - 4 * log(x) - 5);
    }
    cout << "Корень по методу итераций: " << x << endl;
}
