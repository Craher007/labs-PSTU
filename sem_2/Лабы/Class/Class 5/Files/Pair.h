#pragma once
#include <iostream>
#include "Object.h"

using namespace std;

class Pair:public object{

public:
	Pair() { a = 0; b = 0; };
	Pair(int m, int s) { a = m; b = s; }
	Pair(const Pair& t) { a = t.a; b = t.b; }
	~Pair() {};

	int get_a() { return a; }
	int get_b() { return b; }
	void set_a(int m) { a = m; }
	void set_b(int s) { b = s; }

	Pair& operator=(const Pair&);
	Pair operator+(const Pair&);
	
	friend istream& operator>>(istream& in, Pair& t);
	friend ostream& operator<<(ostream& out, const Pair& t);

	void show() override;

protected:
	int a;
	int b;
};