#include <iostream>
#include <cmath>
using namespace std;
int main()
{
    float x, xPrev;
    x = 2; 
    xPrev = 0; 
    double eps = 0.000001;
    while (abs(x - xPrev) > eps) {
        xPrev = x;
        x = xPrev - (3 * x - 4 * log(x) - 5) / (3 - 4 / x);

    }
    cout << "Корень: " << x << endl;


}
