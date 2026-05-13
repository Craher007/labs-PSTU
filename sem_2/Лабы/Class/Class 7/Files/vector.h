#pragma once
#include <iostream>
#include <stdexcept>
using namespace std;

template<class T>
class Vector {
public:

	Vector(int s, T k);
	Vector(const Vector<T>& a);
	~Vector();

	Vector& operator=(const Vector<T>& a);
	Vector<T> operator*(const Vector<T>& a) const;

	T& operator[](int index);
	operator int() const { return size; }

	friend ostream& operator<< <T>(ostream& out, const Vector<T>& a);
	friend istream& operator>> <T>(istream& in, Vector<T>& a);


private:
	int size;
	T* data;
};

template<class T>
Vector<T>::Vector(int s, T k) {
	size = s;
	data = new T[size];
	for (int i = 0; i < size; i++)
		data[i] = k;
}

template<class T>
Vector<T>::Vector(const Vector<T>& a) {
	size = a.size;
	data = new T[size];
	for (int i = 0; i < size; i++)
		data[i] = a.data[i];
}
template<class T>
Vector<T>::~Vector() {
	delete[]data;
	data = 0;
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& a) {
	if (this == &a)
		return *this;

	size = a.size;
	if (data != 0)
		delete[]data;

	data = new T[size];
	for (int i = 0; i < size; i++)
		data[i] = a.data[i];

	return *this;
}


template<class T>
Vector<T> Vector<T>::operator*(const Vector<T>& a) const {
	int limit = size < a.size ? size : a.size;
	Vector<T> temp(limit, T());
	for (int i = 0; i < limit; ++i)
		temp.data[i] = data[i] * a.data[i];
	return temp;
}

template<class T>
T& Vector<T>::operator[](int index) {
	if (index < 0 || index >= size)
		cout << "Индекс выходит за рамки.";
	else return data[index];
}

template<class T>
ostream& operator<<(ostream& out, const Vector<T>& a) {
	for (int i = 0; i < a.size; ++i)
		out << a.data[i] << " ";
	return out;
}

template<class T>
istream& operator>>(istream& in, Vector<T>& a) {
	for (int i = 0; i < a.size; ++i)
		in >> a.data[i];
	return in;
}
