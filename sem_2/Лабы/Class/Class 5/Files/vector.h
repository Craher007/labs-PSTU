#pragma once
#include "Object.h"
#include <string>
#include <iostream>
using namespace std;
class Vector
{
	object** beg;
	int size;
	int cur;
public:
	Vector(void);
	Vector(int);
public:
	~Vector(void);
	void Add(object*);
	friend ostream& operator<<(ostream& out, const Vector&);
	
};