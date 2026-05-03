#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Data {
    string name = "";
    long long passport = 0;
    string address = "";

    bool isDel = false;

    bool isEmpty() {
        return (!isDel && name == "");
    }
};

struct HashTable {
    int size;
    int count;
    Data* table;

    HashTable(int size) {
        this->size = size;
        count = 0;
        table = new Data[size];

    }

    int hash(string key) {
        int hash_key = 0;

        for (char c : key) hash_key += static_cast<int>(c);

        return hash_key % size;
    }

    void puch(Data data) {
        if (size == count) {
            cout << "Невозможно добавить элемент, таблица заполнена!" << endl;
            return;
        }

        int start_idx = hash(data.name);
        int idx = start_idx;

        while (!table[idx].isEmpty() && !table[idx].isDel) {
            if (data.name == table[idx].name) {
                table[idx] = data;
                table[idx].isDel = false;
                cout << "Добавлено: " << data.name << endl;
                return;
            }

            idx = (idx + 1) % size;
        }

        table[idx] = data;
        table[idx].isDel = false;
        count++;

        cout << "Добавлено: " << data.name << endl;
    }

    void search(string name) {
        int start_idx = hash(name);
        int idx = start_idx;

        while (!table[idx].isEmpty()) {
            if (!table[idx].isDel && table[idx].name == name) {
                cout << "Данные найдены: " << table[idx].name << " " << table[idx].passport << " " << table[idx].address << endl;
                return;
            }

            idx = (idx + 1) % size;

            if (idx == start_idx) break;
        }

        cout << "Человек в таблице не найден" << endl;
    }

    void pop(string name) {
        int start_idx = hash(name);
        int idx = start_idx;

        while (!table[idx].isEmpty()) {
            if (!table[idx].isDel && table[idx].name == name) {
                table[idx].isDel = true;
                count--;
                cout << "Человек с именем: " << name << " удален" << endl;
                return;
            }

            idx = (idx + 1) % size;

            if (idx == start_idx) break;
        }

        cout << "Человек в таблице не найден" << endl;
    }

    void clear_table() {
        delete[] table;
        table = new Data[size];
        count = 0;
    }

    void print_table() {
        if (count == 0) {
            cout << "Ошибка, таблица пустая " << endl;
            return;
        }

        for (int i = 0; i < size; i++) {
            cout << i << ": ";

            if (!table[i].isEmpty() && !table[i].isDel) {
                cout << table[i].name << " " << table[i].passport << " " << table[i].address << endl;
            }
            else {
                cout << "Данная ячейка пустая" << endl;
            }
        }
    }

    void load_from_file(string filename) {
        ifstream file(filename);

        if (!file.is_open()) {
            cout << "Ошибка чтения файла!" << endl;
            return;
        }

        Data data;

        while (file >> data.name >> data.passport >> data.address) {
            data.isDel = false;
            puch(data);
        }

        file.close();
    }
};

int main() {
    setlocale(LC_ALL, "");
    HashTable hashTable(100);

    hashTable.load_from_file("input.txt");

    cout << endl;
    hashTable.print_table();

    cout << endl;

    int n;
    cout << "Сколько людей вы хотите найти: ";
    cin >> n;
    cout << endl;

    for (int i = 0; i < n; i++) {
        string name_search;
        cout << "Введите имя: ";
        cin >> name_search;
        hashTable.search(name_search);
    }

    cout << endl;

    int k;
    cout << "Сколько людей вы хотите удалить: ";
    cin >> k;
    cout << endl;

    for (int i = 0; i < k; i++) {
        string name_pop;
        cout << "Введите имя: ";
        cin >> name_pop;
        hashTable.pop(name_pop);
    }

    cout << endl;
    hashTable.print_table();

    hashTable.clear_table();

}