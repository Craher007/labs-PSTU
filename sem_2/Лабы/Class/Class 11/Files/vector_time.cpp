#include "Tasks.h"
#include "Time.h"

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

namespace vector_time{
    template <class T>
    class Vector{
    public:
        Vector() = default;

        explicit Vector(int n){
            for (int i = 0; i < n; ++i){
                T value;
                cout << "Элемент времени [" << i << "]" << endl;
                cin >> value;
                v.push_back(value);
            }
        }

        void Print() const{
            print_container(v);
        }

        T Min() const{
            return *min_element(v.begin(), v.end());
        }

        T Max() const{
            return *max_element(v.begin(), v.end());
        }

        T Average() const{
            T sum{};
            for (const auto& item : v){
                sum = sum + item;
            }
            return sum / static_cast<int>(v.size());
        }

        void AddMin(int pos){
            v.insert(v.begin() + pos, Min());
        }

        void DeleteGreaterThanAverage(){
            T avg = Average();
            v.erase(remove_if(v.begin(), v.end(), [avg](const T& item) {
                return item > avg;
            }), v.end());
        }

        void MultiplyByMax(){
            T mx = Max();
            for (auto& item : v){
                item = item * mx;
            }
        }

        size_t Size() const{
            return v.size();
        }

    private:
        vector<T> v;
    };

    void run(){
        cout << endl<<"Параметризованный класс Vector<Time>"<<endl;
        Vector<Time> v(read_positive_size());
        cout << "Исходный вектор: ";
        v.Print();

        v.AddMin(read_position(v.Size()));
        cout << "После добавления минимального элемента: ";
        v.Print();

        v.DeleteGreaterThanAverage();
        cout << "После удаления элементов больше среднего: ";
        v.Print();

        v.MultiplyByMax();
        cout << "После умножения каждого элемента на максимальный: ";
        v.Print();
    }
}
