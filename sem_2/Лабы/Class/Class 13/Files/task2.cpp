#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
#include <vector>

#include "Time.h"

using namespace std;

template <class T>
class VectorStack : public stack<T, vector<T>>{
public:
    vector<T>& container(){ return this->c; }

    const vector<T>& container() const { return this->c; }
};

typedef VectorStack<Time> TimeStack;

void print_stack(const TimeStack& values){
    const vector<Time>& data = values.container();

    for_each(data.begin(), data.end(), [](const Time& value) {
        cout << value << " ";
    });
    cout << endl;
}

Time average_stack(const TimeStack& values){
    const vector<Time>& data = values.container();
    Time sum = accumulate(data.begin(), data.end(), Time());
    return sum / static_cast<int>(data.size());
}

void add_min_to_position(TimeStack& values){
    vector<Time>& data = values.container();
    vector<Time>::iterator min_it = min_element(data.begin(), data.end());
    int position;

    cout << "Минимальный элемент = " << *min_it << endl;
    cout << "Позиция для добавления: ";
    cin >> position;

    if (position < 0) position = 0;
    if (position > static_cast<int>(data.size())) position = static_cast<int>(data.size());

    data.insert(data.begin() + position, *min_it);
}

void remove_greater_than_average(TimeStack& values){
    vector<Time>& data = values.container();
    Time avg = average_stack(values);

    cout << "Среднее арифметическое = " << avg << endl;

    vector<Time>::iterator new_end = remove_if(data.begin(), data.end(), [avg](const Time& value) {
        return value > avg;
    });
    data.erase(new_end, data.end());
}

void multiply_by_max(TimeStack& values){
    vector<Time>& data = values.container();
    Time max_value = *max_element(data.begin(), data.end());

    cout << "Максимальный элемент = " << max_value << endl;

    transform(data.begin(), data.end(), data.begin(), [max_value](const Time& value) {
        return value * max_value;
    });
}

void task2(){
    int n;
    cout << endl << "Задача 2. Адаптер stack<Time, vector<Time>>" << endl;
    cout << "Количество элементов: ";
    cin >> n;

    if (n <= 0){
        cout << "Контейнер пуст. Выполнение задачи невозможно." << endl;
        return;
    }

    TimeStack values;

    for (int i = 0; i < n; ++i){
        cout << "Элемент " << i + 1 << endl;
        Time value;
        cin >> value;
        values.push(value);
    }

    cout << "Исходный контейнер: ";
    print_stack(values);

    cout << endl << "Добавление минимального элемента на заданную позицию" << endl;
    add_min_to_position(values);
    cout << "После добавления: ";
    print_stack(values);

    cout << endl << "Удаление элементов больше среднего арифметического" << endl;
    remove_greater_than_average(values);
    cout << "После удаления: ";
    print_stack(values);

    if (!values.container().empty()){
        cout << endl << "Умножение каждого элемента на максимальный элемент" << endl;
        multiply_by_max(values);
        cout << "После умножения: ";
        print_stack(values);
    }
}
