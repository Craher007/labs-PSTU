#include "Tasks.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>

using namespace std;

namespace spisok_float{
    using Container = list<float>;

    Container make_container(int n){
        Container c;
        for (int i = 0; i < n; ++i){
            float value;
            cout << "Элемент [" << i << "] = ";
            cin >> value;
            c.push_back(value);
        }
        return c;
    }

    float min_value(const Container& c){
        return *min_element(c.begin(), c.end());
    }

    float max_value(const Container& c){
        return *max_element(c.begin(), c.end());
    }

    float average(const Container& c){
        float sum = accumulate(c.begin(), c.end(), 0.0f);
        return sum / static_cast<float>(c.size());
    }

    void add_min(Container& c, int pos){
        auto it = c.begin();
        advance(it, pos);
        c.insert(it, min_value(c));
    }

    void delete_greater_than_average(Container& c){
        float avg = average(c);
        c.remove_if([avg](float value) { return value > avg; });
    }

    void multiply_by_max(Container& c){
        float mx = max_value(c);
        for (auto& item : c) item *= mx;
    }

    void run(){
        cout << endl <<"Список float"<<endl;
        Container c = make_container(read_positive_size());
        cout << "Исходный список: ";
        print_container(c);

        add_min(c, read_position(c.size()));
        cout << "После добавления минимального элемента: ";
        print_container(c);

        delete_greater_than_average(c);
        cout << "После удаления элементов больше среднего: ";
        print_container(c);

        multiply_by_max(c);
        cout << "После умножения каждого элемента на максимальный: ";
        print_container(c);
    }
}
