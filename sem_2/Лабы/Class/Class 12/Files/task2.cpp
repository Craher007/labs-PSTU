#include <iostream>
#include <iterator>
#include <set>
#include "Time.h"

using namespace std;

typedef set<Time> TSetTime;

TSetTime make_set_time(int n){
    TSetTime s;
    Time a;

    for (int i = 0; i < n; i++){
        cin >> a;
        s.insert(a);
    }

    return s;
}

void print_set_time(const TSetTime& s){
    for (TSetTime::const_iterator it = s.begin(); it != s.end(); ++it)
        cout << *it << " ";
    cout << endl;
}

Time srednee_time(const TSetTime& s){
    Time sum;

    for (TSetTime::const_iterator it = s.begin(); it != s.end(); ++it)
        sum = sum + *it;

    return sum / static_cast<int>(s.size());
}

void add_min_time(TSetTime& s){
    Time mn = *s.begin();
    int pos;

    cout << "Минимальный элемент = " << mn << endl;
    cout << "Позиция: ";
    cin >> pos;

    if (pos < 0) pos = 0;
    if (pos > static_cast<int>(s.size())) pos = static_cast<int>(s.size());

    TSetTime::iterator it = s.begin();
    advance(it, pos);
    s.insert(it, mn);
}

void del_more_srednee_time(TSetTime& s){
    Time sr = srednee_time(s);
    TSetTime::iterator it = s.begin();

    cout << "Среднее арифметическое = " << sr << endl;

    while (it != s.end()){
        if (*it > sr) it = s.erase(it);
        else it++;
    }
}

void multiply_by_max_time(TSetTime& s){
    Time mx = *s.rbegin();
    TSetTime result;

    cout << "Максимальный элемент = " << mx << endl;

    for (TSetTime::iterator it = s.begin(); it != s.end(); ++it)
        result.insert((*it) * mx);

    s = result;
}

void task2(){
    int n;
    cout << "Количество элементов: ";
    cin >> n;

    TSetTime s = make_set_time(n);
    cout << "Исходный контейнер: ";
    print_set_time(s);

    cout << "Добавление минимального элемента:" << endl;
    add_min_time(s);
    print_set_time(s);

    cout << "Удаление элементов больше среднего арифметического:" << endl;
    del_more_srednee_time(s);
    print_set_time(s);

    cout << "Умножение каждого элемента на максимальный:" << endl;
    multiply_by_max_time(s);
    print_set_time(s);
}
