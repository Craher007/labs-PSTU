#include <iostream>
#include "vector.h"
#include "Time.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "");
	Time t;
	cin >> t;
	cout << "t: " << t << endl;

	Vector<Time> a(5, t);
	cout << "a: " << a << endl;

	cout << "Введите 5 элементов a: ";
	cin >> a;
	cout << "a: " << a << endl;

	cout << "a[2] " << a[2] << endl;

	Vector<Time> b(5, t);
	b = a;
	cout << "b = a: " << b << endl;

	cout << "Введите 5 элементов b: ";
	cin >> b;
	cout << "b: " << b << endl;

	Vector<Time> c(5, t);
	c = a * b;
	cout << "a * b: " << c << endl;
}
