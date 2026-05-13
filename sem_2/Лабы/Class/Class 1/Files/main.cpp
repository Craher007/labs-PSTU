#include <iostream>
#include "Header.h"


using namespace std;
int main()
{
    setlocale(LC_ALL, "");
    int hour, minute;
    Time tm;

    cout << "Введите количество часов: ";
    cin >> hour;
    cout << endl << "Введите количество минут: ";
    cin >> minute;
    tm.Init(hour, minute);
    cout << endl << "Общее время в минутах: " << tm.calculation();
}

