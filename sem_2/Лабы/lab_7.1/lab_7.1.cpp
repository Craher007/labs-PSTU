#include <iostream>
#include <complex>

using namespace std;

float multi(float a, float b) {
    return a * b;
}
complex<double> multi(complex<double> a, complex<double> b) {
    return a * b;
}


int main()
{
    complex<double> a1=(3,6);
    complex<double> b1 = (6, 2);
    float a = 5.3, b = 5.2;
    cout << multi(a, b) << endl;
    cout << multi(a1, b1);
    
    
}
