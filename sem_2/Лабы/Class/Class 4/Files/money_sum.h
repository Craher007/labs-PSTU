#pragma once
#include <iostream>
#include "Pair.h"

using namespace std;

class money:public Pair{
public:
	money() { rub = 0; kop = 0; };
	money(int m, int s):Pair(a,b) { rub = m; kop = s; }
	money(const money& t) { a = t.a; b = t.b; rub = t.rub; kop = t.kop; }
	~money() {};

	int get_a() { return rub; }
	int get_b() { return kop; }
	void set_a(int m) { rub = m; }
	void set_b(int s) { kop = s; }

	money& operator=(const money&);
	money operator+(const money&);
	money operator-(const money&);
	money operator/(const money&);

	friend istream& operator>>(istream& in, money& t);
	friend ostream& operator<<(ostream& out, const money& t);

protected:
	int rub;
	int kop;
};