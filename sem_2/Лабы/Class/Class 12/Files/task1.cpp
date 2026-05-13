#include <iostream>
#include <iterator>
#include <set>

using namespace std;

typedef set<float> TSetFloat;

TSetFloat make_set_float(int n){
    TSetFloat s;
    float a;

    for (int i = 0; i < n; i++){
        cout << "Элемент: ";
        cin >> a;
        s.insert(a);
    }

    return s;
}

void print_set_float(const TSetFloat& s){
    for (TSetFloat::const_iterator it = s.begin(); it != s.end(); ++it)
        cout << *it << " ";
    cout << endl;
}

float srednee_float(const TSetFloat& s){
    float sum = 0;

    for (TSetFloat::const_iterator it = s.begin(); it != s.end(); ++it)
        sum += *it;

    return sum / s.size();
}

void add_min_float(TSetFloat& s){
    float mn = *s.begin();
    int pos;

    cout << "Минимальный элемент = " << mn << endl;
    cout << "Позиция: ";
    cin >> pos;

    if (pos < 0) pos = 0;
    if (pos > static_cast<int>(s.size())) pos = static_cast<int>(s.size());

    TSetFloat::iterator it = s.begin();
    advance(it, pos);
    s.insert(it, mn);
}

void del_more_srednee_float(TSetFloat& s){
    float sr = srednee_float(s);
    TSetFloat::iterator it = s.begin();

    cout << "Среднее арифметическое = " << sr << endl;

    while (it != s.end()){
        if (*it > sr) it = s.erase(it);
        else it++;
    }
}

void multiply_by_max_float(TSetFloat& s){
    float mx = *s.rbegin();
    TSetFloat result;

    cout << "Максимальный элемент = " << mx << endl;

    for (TSetFloat::iterator it = s.begin(); it != s.end(); ++it)
        result.insert((*it) * mx);

    s = result;
}

void task1(){
    int n;
    cout << "Количество элементов: ";
    cin >> n;

    TSetFloat s = make_set_float(n);
    cout << "Исходный контейнер: ";
    print_set_float(s);

    cout << "Добавление минимального элемента:" << endl;
    add_min_float(s);
    print_set_float(s);

    cout << "Удаление элементов больше среднего арифметического:" << endl;
    del_more_srednee_float(s);
    print_set_float(s);

    cout << "Умножение каждого элемента на максимальный:" << endl;
    multiply_by_max_float(s);
    print_set_float(s);
}
