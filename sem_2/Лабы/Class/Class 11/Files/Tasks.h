#pragma once

#include <cstddef>
#include <iostream>
#include <stdexcept>

using namespace std;

inline int read_positive_size(){
    int n;
    cout << "Количество элементов = ";
    cin >> n;
    if (n <= 0){
        throw invalid_argument("количество элементов должно быть положительным");
    }
    return n;
}

inline int read_position(size_t size){
    int pos;
    cout << "Позиция вставки (0.." << size << ") = ";
    cin >> pos;
    if (pos < 0 || pos > static_cast<int>(size)){
        throw out_of_range("неверная позиция");
    }
    return pos;
}

template <class Container>
void print_container(const Container& c){
    if (c.empty()) cout << "пусто";
    for (const auto& item : c){
        cout << item << ' ';
    }
    cout << endl;
}

namespace spisok_float{
    void run();
}

namespace spisok_time{
    void run();
}

namespace vector_time{
    void run();
}

namespace stek_time{
    void run();
}

namespace klass_stek_time{
    void run();
}
