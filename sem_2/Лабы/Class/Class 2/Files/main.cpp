#include <iostream>
#include "Header.h"


using namespace std;
int main()
{
    setlocale(LC_ALL, "");
    double a, b, c;
    cout << "Введите коэфиценты уравнения: "<<endl;
    
    cout << "A: ";
    cin >> a;

    cout << "B: ";
    cin >> b;

    cout << "C: ";
    cin >> c;

    equation c1;
    c1.show();

    equation c2(a,b,c);
    c2.show();

    equation c3 = c2;

    c3.set_A(a + 100);
    c3.set_B(b + 100);
    c3.set_C(c + 100);

    c3.show();

}

