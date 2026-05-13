#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>

#include "Time.h"

using namespace std;

typedef multiset<Time> TimeSet;

TimeSet make_set(int n){
    TimeSet values;

    for (int i = 0; i < n; ++i){
        cout << "Элемент " << i + 1 << endl;
        Time value;
        cin >> value;
        values.insert(value);
    }

    return values;
}

void print_set(const TimeSet& values){
    for_each(values.begin(), values.end(), [](const Time& value) {
        cout << value << " ";
    });
    cout << endl;
}

Time average_set(const TimeSet& values){
    Time sum = accumulate(values.begin(), values.end(), Time());
    return sum / static_cast<int>(values.size());
}

void add_min_to_position(TimeSet& values){
    TimeSet::iterator min_it = min_element(values.begin(), values.end());
    int position;

    cout << "Минимальный элемент = " << *min_it << endl;
    cout << "Позиция для добавления: ";
    cin >> position;

    if (position < 0) position = 0;
    if (position > static_cast<int>(values.size())) position = static_cast<int>(values.size());

    TimeSet::iterator insert_it = values.begin();
    advance(insert_it, position);
    values.insert(insert_it, *min_it);

    cout << "Для множества позиция используется как подсказка, итоговый порядок остается отсортированным." << endl;
}

void remove_greater_than_average(TimeSet& values){
    Time avg = average_set(values);
    TimeSet result;

    cout << "Среднее арифметическое = " << avg << endl;

    remove_copy_if(values.begin(), values.end(), inserter(result, result.end()), [avg](const Time& value) {
        return value > avg;
    });

    values = result;
}

void multiply_by_max(TimeSet& values){
    Time max_value = *max_element(values.begin(), values.end());
    TimeSet result;

    cout << "Максимальный элемент = " << max_value << endl;

    transform(values.begin(), values.end(), inserter(result, result.end()), [max_value](const Time& value) {
        return value * max_value;
    });

    values = result;
}

void task3(){
    int n;
    cout << endl << "Задача 3. Ассоциативный контейнер multiset<Time>" << endl;
    cout << "Количество элементов: ";
    cin >> n;

    if (n <= 0){
        cout << "Контейнер пуст. Выполнение задачи невозможно." << endl;
        return;
    }

    TimeSet values = make_set(n);

    cout << "Исходный контейнер: ";
    print_set(values);

    cout << endl << "Добавление минимального элемента на заданную позицию" << endl;
    add_min_to_position(values);
    cout << "После добавления: ";
    print_set(values);

    cout << endl << "Удаление элементов больше среднего арифметического" << endl;
    remove_greater_than_average(values);
    cout << "После удаления: ";
    print_set(values);

    if (!values.empty())
    {
        cout << endl << "Умножение каждого элемента на максимальный элемент" << endl;
        multiply_by_max(values);
        cout << "После умножения: ";
        print_set(values);
    }
}
