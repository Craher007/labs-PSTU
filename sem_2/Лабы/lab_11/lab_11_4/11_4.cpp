
#include <iostream>
using namespace std;

struct queueEl {
    int data;
    queueEl* next;

};


struct Queue {
    queueEl* first;
    queueEl* last;
    int size;

    void init() {
        first = nullptr;
        last = nullptr;
        size = 0;
    }

    void push(int number) {
        queueEl* newqueue = new queueEl;
        newqueue->data = number;
        newqueue->next = nullptr;
        
        if (size == 0) {
            first = newqueue;
            last = newqueue;
        }
        else {
            last->next = newqueue;
            last = newqueue;
        }

        size++;
    }

    int pop() {
        if (size == 0) return 0;
        int data = first->data;
        queueEl* tmp = first;
        if (size == 1) last = nullptr;
        first = first->next;
        size--;
        delete tmp;
        return data;

    }

    void clear() {
        while (size > 0) pop();
    }
    queueEl* firstEl() { return first; }
    queueEl* lastEl() { return last; }
};

Queue* create(int number) {
    Queue* tmp = new Queue;
    tmp->init();
    for (int i = 0; i < number; i++) {
        int x;
        cout << "Введите " << i + 1 << " элемента очереди: ";
        cin >> x;
        tmp->push(x);
    }
    return tmp;
}


void printqueue(Queue& Que) {
    Queue tmp;
    tmp.init();

    while (Que.size > 0) {
        int x = Que.pop();
        cout << x << " ";
        tmp.push(x);
    }
    cout << endl;
    while (tmp.size > 0) Que.push(tmp.pop());
}

void popelem(Queue& Que, int number) {
    if (number<1 or number>Que.size) {
        cout << "Удаление невозможно";
        return;
    }

    for (int i = 0; i <= Que.size; i++) {
        int x = Que.pop();
        if (i == number - 1) continue;
        else Que.push(x);
    }

}

void pushelem(Queue& Que, int number, int count) {
    if (number<1 or number>Que.size) {
        cout << "Добавление невозможно";
        return;
    }

    int siz = Que.size;
    for (int i = 0; i < siz; i++) {
        int x = Que.pop();
        if (i == number-1 ) {
            for (int j = 0; j < count; j++) {
                int y;
                cout << "Введите добавляемый элемент: ";
                cin >> y;
                Que.push(y);
            }
        }
        Que.push(x);
    }


}

int main()
{
    setlocale(LC_ALL, "Russian");
    int n;
    cout << "Введите длину желаемой очереди: ";
    cin >> n;

    Queue* Que= create(n);
    

    int k;
    cout << "Введите номер элемента, который хотите удалить: ";
    cin >> k;

    popelem(*Que, k);

    printqueue(*Que);

    int count;
    cout << "Введите количество добавляемых элементов: ";
    cin >> count;
    pushelem(*Que, k, count);

    printqueue(*Que);
}
