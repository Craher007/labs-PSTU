#include "Tasks.h"
#include "Time.h"

#include <algorithm>
#include <iostream>
#include <list>

using namespace std;

namespace spisok_time{
    using Container = list<Time>;

    Container make_container(int n){
        Container c;
        for (int i = 0; i < n; ++i){
            Time value;
            cout << "Элемент времени [" << i << "]" << endl;
            cin >> value;
            c.push_back(value);
        }
        return c;
    }

    Time min_value(const Container& c){
        return *min_element(c.begin(), c.end());
    }

    Time max_value(const Container& c){
        return *max_element(c.begin(), c.end());
    }

    Time average(const Container& c){
        long long sum = 0;
        for (const auto& item : c){
            sum += item.total_seconds();
        }
        return Time::from_seconds(sum / static_cast<int>(c.size()));
    }

    void add_min(Container& c, int pos){
        auto it = c.begin();
        advance(it, pos);
        c.insert(it, min_value(c));
    }

    void delete_greater_than_average(Container& c){
        Time avg = average(c);
        c.remove_if([avg](const Time& value) { return value > avg; });
    }

    void multiply_by_max(Container& c){
        Time mx = max_value(c);
        for (auto& item : c){
            item = item * mx;
        }
    }

    void run(){
        cout << endl<<"Список Time"<<endl;
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
