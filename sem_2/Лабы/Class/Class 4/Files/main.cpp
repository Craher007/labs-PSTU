#include <iostream>
#include "Pair.h"
#include "money_sum.h"


using namespace std;
int main()
{
    setlocale(LC_ALL, "");
    Pair a;
    Pair b;
    cin >> a;
    cin >> b;
    cout << "Сложение пар: " << a + b;

    money c;
    money d;
    cin >> c;
    cin >> d;
    cout << endl;
    cout << "Сложение: " << endl << c + d << endl;
    cout << "Вычитание: " << endl << c - d << endl;
    cout << "Деление: " << endl << c / d << endl;
    cout <<endl<<c<<endl<<d;

}

