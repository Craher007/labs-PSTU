
#include <iostream>
using namespace std;

struct list {
    int data;
    list* next;
};

list* creatList(int n) {
    if (n <= 0) return nullptr;
    list* start = new list;
    cout << "Введите 1 элемент списка: " << endl;
    cin >> start->data;
    start->next = nullptr;

    list* newlist = nullptr;
    list* pred = start;

    for (int i = 1; i < n; i++) {
        newlist = new list;
        cout << "Введите "<< i + 1<< " элемент списка:" << endl;
        cin >> newlist->data;
        newlist->next = nullptr;

        pred->next = newlist;
        pred = newlist;

    }
    return start;
}

void printlist(list* start) {
    if (start == nullptr) {
        cout << "Список пуст";
        return;
    }

    list* newlist = start;
    while (newlist != nullptr) {
        cout << newlist->data << " ";
        newlist = newlist->next;
    }
    cout << endl;
}

void pop(list*& start, int n) {
    if (start == nullptr or n <= 0) {
        cout << "Удаление не возможно";
        return;
    }
    if (n == 1) {
        list* tmp = start;
        start = start->next;
        delete tmp;
        return;
    }
    list* pred = start;
    for (int i = 1; i < n - 1 and pred != nullptr; i++) {
        pred = pred->next;
    }
    if (pred->next == nullptr or pred == nullptr) {
        cout << "Удаление не возможно";
        return;
    }

    list* del = pred->next;
    pred->next = del->next;
    delete del;

}

void push(list*& start, int k, int n) {
    if (n <= 0 or k <= 0) {
        cout << "Добавление не возможно";
        return;
    }

    list* firstlist = nullptr;
    list* lastlist = nullptr;

    for (int i = 0; i < k; i++) {
        list* newlist = new list;
        cout << "Введите элемент для добавления: ";
        cin >> newlist->data;
        newlist->next = nullptr;

        if (firstlist == nullptr) {
            firstlist = newlist;
            lastlist = newlist;
        }
        else {
            lastlist->next = newlist;
            lastlist = newlist;
        }
    }
    if (n == 1) {
        lastlist->next = start;
        start = firstlist;
        return;
    }

    list* pred = start;
    for (int i = 1; i < n - 1 and pred != nullptr; i++) {
        pred = pred->next;
    }
    if (pred == nullptr) {
        cout << "Добавление не возможно";
        return;
    }

    lastlist->next = pred->next;
    pred->next = firstlist;



}

int main()
{
    setlocale(LC_ALL, "Russian");
    int n;
    cout << "Введите длину желаемого списка: ";
    cin >> n;

    list* list = creatList(n);
    
    printlist(list);

    int k;
    cout << "Введите номер элемента, которых хотите удалить: ";
    cin >> k;

    pop(list, k);

    printlist(list);

    int col;
    cout << "Введите количество добавляемых элементов: ";
    cin >> col;
    push(list, col, k);

    printlist(list);
}
