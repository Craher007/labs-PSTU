#pragma once
#include <iostream>
using namespace std;

class Vector {
public:
	class Iterator {
		friend class Vector;
	public:
		Iterator(){ elem = 0; }
		Iterator(const Iterator& it)  {elem = it.elem; }
		Iterator operator+(int n) const;
		int& operator*() const{ return *elem; }
	private:
		int* elem;
	};

	Vector(int s, int k = 0);
	Vector(const Vector& a);
	~Vector();

	Vector& operator=(const Vector& a);
	Vector& operator*(const Vector& a);

	int& operator[](int index) { return size; }
	operator int() const;

	friend ostream& operator<<(ostream& out, const Vector& a);
	friend istream& operator>>(istream& in, Vector& a);

	Iterator first() { return beg; }
	Iterator last() { return end; }

private:
	void resetIterators();

	int size;
	int* data;
	Iterator beg;
	Iterator end;
};
