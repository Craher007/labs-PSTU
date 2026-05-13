#include <clocale>
#include <iostream>
#include <windows.h>


using namespace std;

void task1();
void task2();
void task3();

int main(){
    setlocale(LC_ALL, "");
    int choice;

    do{
        cout << endl << "Лабораторная работа 13. Вариант 5" << endl;
        cout << "1. Задача 1: list<Time>" << endl;
        cout << "2. Задача 2: stack<Time, vector<Time>>" << endl;
        cout << "3. Задача 3: multiset<Time>" << endl;
        cout << "0. Выход" << endl;
        cout << "Выбор: ";
        cin >> choice;

        switch (choice){
        case 1:
            task1();
            break;
        case 2:
            task2();
            break;
        case 3:
            task3();
            break;
        case 0:
            cout << "Работа завершена." << endl;
            break;
        default:
            cout << "Неверный выбор." << endl;
            break;
        }
    } while (choice != 0);

    return 0;
}
