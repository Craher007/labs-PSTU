#include "Header.h"
#include <iostream>

using namespace std;

equation::equation() {
	A = 0;
	B = 0;
	C = 0;
	cout << "Конструктор без параметров" << endl;
}
equation::equation(double a, double b, double c) {
	A = a;
	B = b;
	C = c;
	cout << "Конструктор с параметрами" << endl;
}
equation::equation(const equation& t) {
	A = t.A;
	B = t.B;
	C = t.C;
	cout << "Конструктор копирования" << endl;
}

equation::~equation() { cout << "Деструктор"<<endl; }

double equation::get_A() { return A; }
double equation::get_B() { return B; }
double equation::get_C() { return C; }

void equation::set_A(double a) { A = a; }
void equation::set_B(double b) { B = b; }
void equation::set_C(double c) { C = c; }

void equation::show() {
	cout << "A: " << A<<endl;
	cout << "B: " << B << endl;
	cout << "C: " << C << endl;
}