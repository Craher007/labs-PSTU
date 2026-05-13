#include <iostream>
#include "vector.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "");

	Vector a(5);
	cout << "a: " << a << endl;

	cout << "Введите 5 жлементов: ";
	cin >> a;
	cout << "a: " << a << endl;

	a[2] = 100;
	cout << "a[2] = 100: " << a << endl;

	Vector b(10);
	b = a;
	cout << "b = a: " << b << endl;

	Vector c(10);
	cout << "Введите 10 элементов: ";
	cin >> c;

	c = c * b;
	cout << "c * b: " << c << endl;

	Vector::Iterator it = c.first();
	it = it + 2;
	cout << "Второй элементt: " << *it << endl;
}
