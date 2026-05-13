#include "vector.h"
#include "error.h"

using namespace std;

Vector::Iterator::Iterator() {
    elem = 0;
    finish = 0;
}

Vector::Iterator::Iterator(const Iterator& it) {
    elem = it.elem;
    finish = it.finish;
}

Vector::Iterator Vector::Iterator::operator+(int n) const {
    if (n < 0 || elem == 0 || finish == 0 || n >= finish - elem)
        throw 5;

    Iterator temp(*this);
    temp.elem += n;
    return temp;
}

int& Vector::Iterator::operator*() const {
    if (elem == 0 || elem >= finish) throw 5;

    return *elem;
}

Vector::Vector(int s, int k) {
    if (s < 0) throw 6;
    if (s > MAX_SIZE) throw 1;

    size = s;
    data = size == 0 ? 0 : new int[size];
    for (int i = 0; i < size; i++)
        data[i] = k;
    resetIterators();
}

Vector::Vector(const Vector& a) {
    size = a.size;
    data = size == 0 ? 0 : new int[size];
    for (int i = 0; i < size; i++)
        data[i] = a.data[i];
    resetIterators();
}

Vector::~Vector() {
    delete[] data;
    data = 0;
}

Vector& Vector::operator=(const Vector& a) {
    if (this == &a) return *this;

    delete[] data;

    size = a.size;
    data = size == 0 ? 0 : new int[size];
    for (int i = 0; i < size; i++)
        data[i] = a.data[i];

    resetIterators();
    return *this;
}

Vector Vector::operator*(const Vector& a) const {
    if (size != a.size) throw 4;

    Vector result(size);
    for (int i = 0; i < size; ++i)
        result.data[i] = data[i] * a.data[i];

    return result;
}

int& Vector::operator[](int index) {
    if (index < 0) throw 2;
    if (index >= size) throw 3;

    return data[index];
}

const int& Vector::operator[](int index) const {
    if (index < 0) throw 2;
    if (index >= size) throw 3;

    return data[index];
}

Vector::operator int() const {
    return size;
}

void Vector::resetIterators() {
    int* afterLast = data == 0 ? 0 : data + size;

    beg.elem = data;
    beg.finish = afterLast;
    end.elem = afterLast;
    end.finish = afterLast;
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
