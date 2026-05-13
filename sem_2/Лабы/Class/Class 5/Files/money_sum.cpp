#include "money_sum.h"
#include <iostream>

using namespace std;

money& money::operator=(const money& t)
{
	if (&t == this) return *this;
	a = t.a;
	b = t.b;
	rub = t.rub;
	kop = t.kop;
	return *this;
}



money money::operator+(const money&t) {
	int tmp1 = rub * 100 + kop;
	int tmp2 = t.rub * 100 + t.kop;
	money p;
	p.a = a + t.a;
	p.b = b + t.b;
	p.rub = (tmp1 + tmp2) / 100;
	p.kop = (tmp1 + tmp2) % 100;
	return p;
}
money money::operator-(const money&t) {
	int tmp1 = rub * 100 + kop;
	int tmp2 = t.rub * 100 + t.kop;
	money p;
	p.a = a + t.a;
	p.b = b + t.b;
	p.rub = (tmp1 - tmp2) / 100;
	p.kop = (tmp1 - tmp2) % 100;
	return p;
}
money money::operator/(const money&t) {
	int tmp1 = rub * 100 + kop;
	int tmp2 = t.rub * 100 + t.kop;
	money p;
	p.a = a - t.a;
	p.b = b - t.b;
	p.rub = (tmp1 / tmp2) / 100;
	p.kop = (tmp1 / tmp2) % 100;
	return p;
}

istream& operator>>(istream& in, money& t) {
	cout << "a: "; in >> t.a;
	cout << "b: "; in >> t.b;
	cout << "rub: "; in >> t.rub;
	cout << "kop: "; in >> t.kop;
	return in;
}
ostream& operator<<(ostream& out, const money& t) {
	return (out <<"a: " << t.a << " b: " << t.b << endl
		<<"rub: " << t.rub << " kop: " << t.kop << endl);
}

void money::show() {
	cout << *this;
}

