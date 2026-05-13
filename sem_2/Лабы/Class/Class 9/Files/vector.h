#pragma once
#include <iostream>

const int MAX_SIZE = 30;

class Vector {
public:
    class Iterator {
        friend class Vector;
    public:
        Iterator();
        Iterator(const Iterator& it);
        Iterator operator+(int n) const;
        int& operator*() const;
    private:
        int* elem;
        int* finish;
    };

    Vector(int s = 0, int k = 0);
    Vector(const Vector& a);
    ~Vector();

    Vector& operator=(const Vector& a);
    Vector operator*(const Vector& a) const;

    int& operator[](int index);
    const int& operator[](int index) const;
    operator int() const;

    friend std::ostream& operator<<(std::ostream& out, const Vector& a);
    friend std::istream& operator>>(std::istream& in, Vector& a);

    Iterator first() { return beg; }
    Iterator last() { return end; }

private:
    void resetIterators();

    int size;
    int* data;
    Iterator beg;
    Iterator end;
};
