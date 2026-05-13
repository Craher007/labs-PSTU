#include <iostream>
#include "Time.h"
#include "Vector.h"

using namespace std;

void task3(){
    int n;
    cout << "Количество элементов: ";
    cin >> n;

    Vector<Time> v(n);
    cout << "Исходный контейнер: ";
    v.Print();

    int pos;
    cout << "Добавление минимального элемента:" << endl;
    cout << "Позиция: ";
    cin >> pos;
    v.AddMin(pos);
    v.Print();

    cout << "Удаление элементов больше среднего арифметического:" << endl;
    v.DelMoreSrednee();
    v.Print();

    cout << "Умножение каждого элемента на максимальный:" << endl;
    v.MultiplyByMax();
    v.Print();
}
