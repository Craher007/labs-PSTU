#include "vector.h"

Vector::Vector(void)
{
	beg = 0;
	size = 0;
	cur = 0;
}
Vector::~Vector(void)
{
	if (beg != 0)delete[] beg;
	beg = 0;
}
Vector::Vector(int n)
{
	beg = new object * [n];
	cur = 0;
	size = n;
}

void Vector::Add(object* p)
{
	if (cur < size)
	{
		beg[cur] = p;
		cur++;
	}
}

ostream& operator<<(ostream& out, const Vector& v)
{
	if (v.size == 0) out << "Empty" << endl;
	object** p = v.beg;
	for (int i = 0; i < v.cur; i++)
	{
		(*p)->show();
		p++;
	}
	return out;
}