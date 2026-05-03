
#include <iostream>
using namespace std;

struct st {
    int data;
    st* next;

};


struct stack {
    st* top;
    int size;

    void init() {
        top = nullptr;
        size = 0;
    }

    void push(int number) {
        st* newstack = new st;
        newstack->data = number;
        newstack->next = top;
        top = newstack;
        size++;
    }

    int pop() {
        if (size == 0) return 0;
        int data = top->data;
        st* tmp = top;
        top = top->next;
        size--;
        delete tmp;
        return data;

    }

    void clear() {
        while (size > 0) pop();
    }
    st* topE() { return top; }
};

void createStack(stack& stElem, int number) {
    stack tmp;
    tmp.init();

    for (int i = 0; i < number; i++) {
        int x;
        cout << "Введите " << i + 1 << " элемент стека: ";
        cin >> x;
        tmp.push(x);
    }
    while (tmp.size > 0) stElem.push(tmp.pop());
}


void printstack(stack& stElem) {
    stack tmp;
    tmp.init();

    while (stElem.size > 0) {
        int x = stElem.pop();
        cout << x << " ";
        tmp.push(x);
    }
    cout << endl;
    while (tmp.size > 0) stElem.push(tmp.pop());
}

void popelem(stack& stElem, int number) {
    if (number<1 or number>stElem.size) {
        cout << "Удаление невозможно";
        return;
    }

    stack tmp;
    tmp.init();

    for (int i = 1; i < number; i++) {
        tmp.push(stElem.pop());
    }
    stElem.pop();
    while (tmp.size > 0) stElem.push(tmp.pop());

}

void pushelem(stack& stElem, int number, int count) {
    if (number<1 or number>stElem.size) {
        cout << "Добавление невозможно";
        return;
    }
    stack tmp;
    tmp.init();

    for (int i = 1; i < number; i++) {
        tmp.push(stElem.pop());
    }
    
    stack add;
    add.init();

    for (int i = 0; i < count; i++) {
        int x;
        cout << "Введите добавляемый элемент: ";
        cin >> x;
        add.push(x);
    }

    while (add.size > 0) stElem.push(add.pop());
    while (tmp.size > 0) stElem.push(tmp.pop());

}

int main()
{
    setlocale(LC_ALL, "Russian");
    int n;
    cout << "Введите длину желаемого стека: ";
    cin >> n;

    stack stElem;
    stElem.init();
    createStack(stElem, n);

    int k;
    cout << "Введите номер элемента, который хотите удалить: ";
    cin >> k;

    popelem(stElem, k);

    printstack(stElem);

    int count;
    cout << "Введите количество добавляемых элементов: ";
    cin >> count;
    pushelem(stElem, k, count);

    printstack(stElem);
}
