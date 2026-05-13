#include <exception>
#include <iostream>

#include "Tasks.h"

using namespace std;

int main(){
    try{
        cout << "Лабораторная работа 11, вариант 5\n";
        cout << "1 - список float\n";
        cout << "2 - список Time\n";
        cout << "3 - параметризованный класс Vector<Time>\n";
        cout << "4 - стек Time на базе vector\n";
        cout << "5 - параметризованный класс со стеком Time\n";
        cout << "0 - выполнить все\n";
        cout << "Выбор = ";

        int choice;
        cin >> choice;

        switch (choice){
        case 0:
            spisok_float::run();
            spisok_time::run();
            vector_time::run();
            stek_time::run();
            klass_stek_time::run();
            break;
        case 1:
            spisok_float::run();
            break;
        case 2:
            spisok_time::run();
            break;
        case 3:
            vector_time::run();
            break;
        case 4:
            stek_time::run();
            break;
        case 5:
            klass_stek_time::run();
            break;
        default:
            cout << "Неверный выбор" << endl;
            break;
        }
    }
    catch (const exception& error){
        cout << "Ошибка: " << error.what() << endl;
    }
}
