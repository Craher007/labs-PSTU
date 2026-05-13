#include "vector.h"
#include <stdexcept>

Vector::Iterator Vector::Iterator::operator+(int n) const {
	Iterator temp(*this);
	temp.elem += n;
	return temp;
}

Vector::Vector(int s, int k) {
	size = s;
	data = new int[size];
	for (int i = 0; i < size; i++)
		data[i] = k;
	resetIterators();
}

Vector::Vector(const Vector& a) {
	size = a.size;
	data = new int[size];
	for (int i = 0; i < size; i++)
		data[i] = a.data[i];
	resetIterators();
}

Vector::~Vector() {
	delete[]data;
	data = 0;
}

Vector& Vector::operator=(const Vector& a) {
	if (this == &a)
		return *this;

	size = a.size;
	if (data != 0)
		delete[]data;

	data = new int[size];
	for (int i = 0; i < size; i++)
		data[i] = a.data[i];

	resetIterators();
	return *this;
}

Vector& Vector::operator*(const Vector& a) {
	int limit = size < a.size ? size : a.size;
	for (int i = 0; i < limit; ++i)
		data[i] *= a.data[i];
	return *this;
}

int& Vector::operator[](int index) {
	if (index < 0 || index >= size)
		cout << "Индекс выходит за пределы";
	else data[index];
}

void Vector::resetIterators() {
	beg.elem = data;
	end.elem = data + size;
}

ostream& operator<<(ostream& out, const Vector& a) {
	for (int i = 0; i < a.size; ++i)
		out << a.data[i] << " ";
	return out;
}

istream& operator>>(istream& in, Vector& a) {
	for (int i = 0; i < a.size; ++i)
		in >> a.data[i];
	return in;
}
