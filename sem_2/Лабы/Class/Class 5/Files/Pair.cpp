#include "Pair.h"
#include <iostream>

using namespace std;

Pair& Pair::operator=(const Pair& t)
{
	if (&t == this) return *this;
	a = t.a;
	b = t.b;
	return *this;
}

Pair Pair::operator+(const Pair&t) {
	Pair p;
	p.a = a + t.a;
	p.b = b + t.b;
	return p;
}

istream& operator>>(istream& in, Pair& t) {
	cout << "a: "; in >> t.a;
	cout << "b: "; in >> t.b;
	return in;
}
ostream& operator<<(ostream& out, const Pair& t) {
	return (out <<"a: " << t.a << " b: " << t.b << endl);
}

void Pair::show() {
	cout << *this;
}
