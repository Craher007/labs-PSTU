
#include <iostream>
using namespace std;

struct list {
    int data;
    list* next;
    list* prev;
};

list* creatList(int n) {
    if (n <= 0) return nullptr;
    list* start = new list;
    cout << "Введите 1 элемент списка: " << endl;
    cin >> start->data;
    start->next = nullptr;
    start->prev = nullptr;

    list* newlist = nullptr;
    list* pred = start;

    for (int i = 1; i < n; i++) {
        newlist = new list;
        cout << "Введите " << i + 1 << " элемент списка:" << endl;
        cin >> newlist->data;
        newlist->next = nullptr;
        newlist->prev = pred;

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
        if (start != nullptr) start->prev = nullptr;
        return;
    }
    list* del = start;
    for (int i = 0; i < n - 1 and del != nullptr; i++) {
        del = del->next;
    }
    if (del == nullptr) {
        cout << "Удаление не возможно";
        return;
    }
    if (del->prev != nullptr) {
        del->prev->next = del->next;
    }
    else {
        start = del->next;
    }

    if (del->next != nullptr) {
        del->next->prev = del->prev;
    }

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
        newlist->prev = nullptr;

        if (firstlist == nullptr) {
            firstlist = newlist;
            lastlist = newlist;
        }
        else {
            lastlist->next = newlist;
            newlist->prev = lastlist;
            lastlist = newlist;
        }
    }
    if (n == 1) {
        firstlist->prev = nullptr;
        lastlist->next = start;
        if (start != nullptr) start->prev = lastlist;
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
    if (pred->next != nullptr) pred->next->prev = lastlist;
    firstlist->prev = pred;
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

    int count;
    cout << "Введите количество добавляемых элементов: ";
    cin >> count;
    push(list, count, k);

    printlist(list);
}
