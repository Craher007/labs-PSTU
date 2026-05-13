#pragma once
#include <iostream>
#include <iterator>
#include <set>

using namespace std;

template<class T>
class Vector{
    set<T> v;
public:
    Vector() {}
    Vector(int n);

    void Print() const;
    T Srednee() const;
    T Min() const;
    T Max() const;
    void AddMin(int pos);
    void DelMoreSrednee();
    void MultiplyByMax();
};

template<class T>
Vector<T>::Vector(int n){
    T a;
    for (int i = 0; i < n; i++){
        cin >> a;
        v.insert(a);
    }
}

template<class T>
void Vector<T>::Print() const{
    for (typename set<T>::const_iterator it = v.begin(); it != v.end(); ++it)
        cout << *it << " ";
    cout << endl;
}

template<class T>
T Vector<T>::Srednee() const{
    T sum = T();
    for (typename set<T>::const_iterator it = v.begin(); it != v.end(); ++it)
        sum = sum + *it;
    return sum / static_cast<int>(v.size());
}

template<class T>
T Vector<T>::Min() const{ return *v.begin(); }

template<class T>
T Vector<T>::Max() const{ return *v.rbegin(); }

template<class T>
void Vector<T>::AddMin(int pos){
    T m = Min();
    typename set<T>::iterator it = v.begin();

    if (pos < 0) pos = 0;
    if (pos > static_cast<int>(v.size())) pos = static_cast<int>(v.size());
    advance(it, pos);

    v.insert(it, m);
}

template<class T>
void Vector<T>::DelMoreSrednee(){
    T sr = Srednee();
    typename set<T>::iterator it = v.begin();

    while (it != v.end()){
        if (*it > sr) it = v.erase(it);
        else it++;
    }
}

template<class T>
void Vector<T>::MultiplyByMax(){
    T mx = Max();
    set<T> result;

    for (typename set<T>::iterator it = v.begin(); it != v.end(); ++it)
        result.insert((*it) * mx);

    v = result;
}
