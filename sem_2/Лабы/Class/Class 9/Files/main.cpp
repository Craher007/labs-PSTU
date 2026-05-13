#include <iostream>
#include <clocale>
#include "error.h"
#include "vector.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "");

    try {
        Vector a(5);
        cout << "Введите 5 элементов: ";
        cin >> a;
        cout << "a: " << a << endl;
        cout << "Размер a: " << int(a) << endl;

        a[2] = 100;
        cout << "a[2] = 100: " << a << endl;

        Vector b(5, 2);
        cout << "b: " << b << endl;

        Vector c = a * b;
        cout << "a * b: " << c << endl;

        Vector::Iterator it = c.first();
        it = it + 2;
        cout << "Элемент после перехода на 2 позиции: " << *it << endl;
    }
    catch (const Error& e) {
        cout << e.what() << endl;
    }

    cout << endl << "Проверка исключительных ситуаций:" << endl;

    try {
        Vector tooBig(MAX_SIZE + 1);
    }
    catch (const Error& e) {
        cout << e.what() << endl;
    }

    try {
        Vector v(3);
        cout << v[-1] << endl;
    }
    catch (const Error& e) {
        cout << e.what() << endl;
    }

    try {
        Vector v(3);
        cout << v[3] << endl;
    }
    catch (const Error& e) {
        cout << e.what() << endl;
    }

    try {
        Vector a(2, 1);
        Vector b(3, 2);
        Vector c = a * b;
        cout << c << endl;
    }
    catch (const Error& e) {
        cout << e.what() << endl;
    }

    try {
        Vector v(3, 1);
        Vector::Iterator it = v.first();
        it = it + 3;
        cout << *it << endl;
    }
    catch (const Error& e) {
        cout << e.what() << endl;
    }
}
