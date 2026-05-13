#include <iostream>

using namespace std;

void task1();
void task2();
void task3();

int main(){
    setlocale(LC_ALL, "");

    int choice;

    do{
        cout << endl;
        cout << "1. Задача 1: set<float>" << endl;
        cout << "2. Задача 2: set<Time>" << endl;
        cout << "3. Задача 3: Vector<Time>" << endl;
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
            break;
        default:
            cout << "Неверный выбор" << endl;
        }
    } while (choice != 0);
}
