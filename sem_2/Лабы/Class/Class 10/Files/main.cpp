#include <iostream>
#include <fstream>
#include <string>
#include <clocale>
#include "time.h"

void CopyFile(const string& from, const string& to) {
    ifstream file1(from);
    ofstream file2(to);

    Time t;

    while (file1 >> t) file2 << t << endl;

    file1.close();
    file2.close();
}

void CreateFile() {
    ofstream file("text.txt");

    if (!file) {
        cout << "Ошибка открытия файла" << endl;
        return;
    }

    int n;
    cout << "Введите количество записей: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        Time t;

        cout << "Введите время в формате минуты:секунды: ";
        cin >> t;

        file << t << endl;
    }

    file.close();

    cout << "Файл создан" << endl;
}

void PrintFile() {
    ifstream file("text.txt");

    if (!file) {
        cout << "Файл не найден" << endl;
        return;
    }

    Time t;
    int number = 1;
    bool empty = true;

    cout << "Содержимое файла:" << endl;

    while (file >> t) {
        cout << number << ". " << t << endl;
        number++;
        empty = false;
    }

    if (empty) cout << "Файл пуст" << endl;

    file.close();
}

void delifmore() {
    ifstream file("text.txt");
    if (!file) {
        cout << "Файл не найден" << endl;
        return;
    }

    ofstream temp("temp.txt");
    Time k;
    cout << "Введите значение, все записи больше которого будут удалены:  ";
    cin >> k;
    Time tmp;
    while (file >> tmp) {
        if (tmp < k) {
            temp << tmp<<endl;
        }
    }

    file.close();
    temp.close();

    CopyFile("temp.txt", "text.txt");

    cout << "Записи больше заданного элемента удалены" << endl;
}

void Addition() {
    Time m(1, 30);
    ifstream file("text.txt");
    if (!file) {
        cout << "Файл не найден" << endl;
        return;
    }

    ofstream temp("temp.txt");
    Time k;
    cout << "Введите значение, которое нужно увеличить";
    cin >> k;
    Time tmp;
    while (file >> tmp) {
        if (tmp == m) tmp = tmp + m;
        temp << tmp << endl;
    }

    file.close();
    temp.close();

    CopyFile("temp.txt", "text.txt");

    cout << "Подходщие записи увеличины на минуту и 30 секунд" << endl;
}

void AddElemAfterN() {

    ifstream file("text.txt");
    if (!file) {
        cout << "Файл не найден" << endl;
        return;
    }

    ofstream temp("temp.txt");

    Time tmp;
    Time newelem;
    int k, n;
    cout << "Введите количество добавляемых элементов: ";
    cin >> k;
    cout << "Введите номер элемента после которого добавлять элементы: ";
    cin >> n;
    int number = 1;
    while (file >> tmp) {
        if (number == n) {
            for (int i = 0; i < k; i++) {
                cout << "Введите элемент: ";
                cin >> newelem;
                temp << newelem << endl;
            }
        }
        number++;
        temp << tmp << endl;

    }

    file.close();
    temp.close();

    CopyFile("temp.txt", "text.txt");

    cout << "Элементы добавлены" << endl;

}

using namespace std;
int main()
{
    setlocale(LC_ALL, "");
    

}

