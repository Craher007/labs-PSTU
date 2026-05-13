#pragma once

#include "Object.h"

class Vector
{
public:
    Vector();
    explicit Vector(int size);
    Vector(const Vector& other) = delete;
    virtual ~Vector();

    void Add();
    void Del();
    void Show() const;
    int operator()() const { return cur; }
    void HandleEvent(const TEvent& event);

    Vector& operator=(const Vector& other) = delete;

protected:
    Object** beg;
    int size;
    int cur;
};
