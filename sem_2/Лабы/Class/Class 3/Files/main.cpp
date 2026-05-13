#include <iostream>
#include "Header.h"


using namespace std;
int main()
{
    setlocale(LC_ALL, "");
    Time a;
    Time b;
    Time c;
    cout << "Введите минуты и секунды: "<<endl;
    cin >> a;
    cout << "Введите минуты и секунды: " << endl;
    cin >> b;
    c = a - b;
    if (a == b) cout << "Интерваллы равны: ";
    if (a != b)cout << "Интервалы не равны: ";

    cout << a;
    cout << b;
    cout << c;

}

