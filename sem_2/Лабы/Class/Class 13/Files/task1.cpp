#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>

#include "Time.h"

using namespace std;

typedef list<Time> TimeList;

TimeList make_list(int n){
    TimeList values;

    for (int i = 0; i < n; ++i){
        cout << "Элемент " << i + 1 << endl;
        Time value;
        cin >> value;
        values.push_back(value);
    }

    return values;
}

void print_list(const TimeList& values){
    for_each(values.begin(), values.end(), [](const Time& value) {
        cout << value << " ";
    });
    cout << endl;
}

Time average_list(const TimeList& values){
    Time sum = accumulate(values.begin(), values.end(), Time());
    return sum / static_cast<int>(values.size());
}

void add_min_to_position(TimeList& values){
    TimeList::iterator min_it = min_element(values.begin(), values.end());
    int position;

    cout << "Минимальный элемент = " << *min_it << endl;
    cout << "Позиция для добавления: ";
    cin >> position;

    if (position < 0) position = 0;
    if (position > static_cast<int>(values.size())) position = static_cast<int>(values.size());

    TimeList::iterator insert_it = values.begin();
    advance(insert_it, position);
    values.insert(insert_it, *min_it);
}

void remove_greater_than_average(TimeList& values){
    Time avg = average_list(values);
    cout << "Среднее арифметическое = " << avg << endl;

    TimeList::iterator new_end = remove_if(values.begin(), values.end(), [avg](const Time& value) {
        return value > avg;
    });
    values.erase(new_end, values.end());
}

void multiply_by_max(TimeList& values){
    Time max_value = *max_element(values.begin(), values.end());
    cout << "Максимальный элемент = " << max_value << endl;

    transform(values.begin(), values.end(), values.begin(), [max_value](const Time& value) {
        return value * max_value;
    });
}

void task1(){
    int n;
    cout << endl << "Задача 1. Последовательный контейнер list<Time>" << endl;
    cout << "Количество элементов: ";
    cin >> n;

    if (n <= 0){
        cout << "Контейнер пуст. Выполнение задачи невозможно." << endl;
        return;
    }

    TimeList values = make_list(n);

    cout << "Исходный контейнер: ";
    print_list(values);

    cout << endl << "Добавление минимального элемента на заданную позицию" << endl;
    add_min_to_position(values);
    cout << "После добавления: ";
    print_list(values);

    cout << endl << "Удаление элементов больше среднего арифметического" << endl;
    remove_greater_than_average(values);
    cout << "После удаления: ";
    print_list(values);

    if (!values.empty()){
        cout << endl << "Умножение каждого элемента на максимальный элемент" << endl;
        multiply_by_max(values);
        cout << "После умножения: ";
        print_list(values);
    }
}
