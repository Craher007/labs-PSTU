#include "Tasks.h"
#include "Time.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

namespace stek_time{
    using Stack = stack<Time, vector<Time>>;

    vector<Time> stack_to_vector(Stack s){
        vector<Time> result;
        while (!s.empty()){
            result.push_back(s.top());
            s.pop();
        }
        return result;
    }

    Stack vector_to_stack(const vector<Time>& values){
        Stack s;
        for (auto it = values.rbegin(); it != values.rend(); ++it){
            s.push(*it);
        }
        return s;
    }

    Stack make_stack(int n){
        Stack s;
        for (int i = 0; i < n; ++i){
            Time value;
            cout << "Элемент времени [" << i << "]" << endl;
            cin >> value;
            s.push(value);
        }
        return s;
    }

    void print_stack(Stack s){
        if (s.empty()) cout << "пусто";
        while (!s.empty()){
            cout << s.top() << ' ';
            s.pop();
        }
        cout << endl;
    }

    Time average(const vector<Time>& values){
        long long sum = 0;
        for (const auto& item : values){
            sum += item.total_seconds();
        }
        return Time::from_seconds(sum / static_cast<int>(values.size()));
    }

    void add_min(Stack& s, int pos){
        vector<Time> values = stack_to_vector(s);
        Time mn = *min_element(values.begin(), values.end());
        values.insert(values.begin() + pos, mn);
        s = vector_to_stack(values);
    }

    void delete_greater_than_average(Stack& s){
        vector<Time> values = stack_to_vector(s);
        Time avg = average(values);
        values.erase(remove_if(values.begin(), values.end(), [avg](const Time& item) {
            return item > avg;
        }), values.end());
        s = vector_to_stack(values);
    }

    void multiply_by_max(Stack& s){
        vector<Time> values = stack_to_vector(s);
        Time mx = *max_element(values.begin(), values.end());
        for (auto& item : values){
            item = item * mx;
        }
        s = vector_to_stack(values);
    }

    void run(){
        cout << endl<<"Стек Time на базе vector"<<endl;
        Stack s = make_stack(read_positive_size());
        cout << "Исходный стек (с вершины к основанию): ";
        print_stack(s);

        add_min(s, read_position(s.size()));
        cout << "После добавления минимального элемента: ";
        print_stack(s);

        delete_greater_than_average(s);
        cout << "После удаления элементов больше среднего: ";
        print_stack(s);

        multiply_by_max(s);
        cout << "После умножения каждого элемента на максимальный: ";
        print_stack(s);
    }
}
