#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Data {
    string name = "";
    long long passport = 0;
    string address = "";
};

struct Node {
    Data data;
    Node* next;

    Node(Data data) {
        this->data = data;
        this->next = nullptr;
    }
};

struct HashTable {
    int size;
    int count;
    Node** table;

    HashTable(int size) {
        this->size = size;
        count = 0;

        table = new Node * [size];

        for (int i = 0; i < size; i++) {
            table[i] = nullptr;
        }
    }

    int hash(string key) {
        int hash_key = 0;

        for (char c : key) hash_key += static_cast<int>(c);

        return hash_key % size;
    }

    void push(Data data) {
        int idx = hash(data.name);

        Node* current = table[idx];

        while (current != nullptr) {
            if (current->data.name == data.name) {
                current->data = data;
                cout << "Данные обновлены: " << data.name << endl;
                return;
            }

            current = current->next;
        }

        Node* newNode = new Node(data);

        newNode->next = table[idx];
        table[idx] = newNode;

        count++;

        cout << "Добавлено: " << data.name << endl;
    }

    void search(string name) {
        int idx = hash(name);

        Node* current = table[idx];

        while (current != nullptr) {
            if (current->data.name == name) {
                cout << "Данные найдены: " << current->data.name << " " << current->data.passport << " " << current->data.address << endl;
                return;
            }

            current = current->next;
        }

        cout << "Человек в таблице не найден" << endl;
    }

    void pop(string name) {
        int idx = hash(name);

        Node* current = table[idx];
        Node* previous = nullptr;

        while (current != nullptr) {
            if (current->data.name == name) {
                if (previous == nullptr) table[idx] = current->next;
                else previous->next = current->next;

                delete current;
                count--;

                cout << "Человек с именем " << name << " удален" << endl;
                return;
            }

            previous = current;
            current = current->next;
        }

        cout << "Человек в таблице не найден" << endl;
    }

    void clear_table() {
        for (int i = 0; i < size; i++) {
            Node* current = table[i];

            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }

            table[i] = nullptr;
        }

        count = 0;
    }

    void print_table() {
        if (count == 0) {
            cout << "Ошибка, таблица пустая" << endl;
            return;
        }

        for (int i = 0; i < size; i++) {
            cout << i << ": ";

            if (table[i] == nullptr) {
                cout << "Данная ячейка пустая";
            }
            else {
                Node* current = table[i];

                while (current != nullptr) {
                    cout << "[" << current->data.name << " " << current->data.passport << " " << current->data.address  << "]";

                    if (current->next != nullptr) {
                        cout << " -> ";
                    }

                    current = current->next;
                }
            }

            cout << endl;
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
            push(data);
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