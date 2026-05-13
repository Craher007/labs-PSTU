#include "Vector.h"

#include "Magazin.h"
#include "Print.h"

#include <iostream>
using namespace std;

Vector::Vector() : beg(nullptr), size(0), cur(0){}

Vector::Vector(int size) : beg(new Object*[size]), size(size), cur(0)
{
    for (int i = 0; i < size; ++i) beg[i] = nullptr;
}

Vector::~Vector()
{
    for (int i = 0; i < cur; ++i)
    {
        delete beg[i];
    }

    delete[] beg;
}

void Vector::Add()
{
    if (beg == nullptr)
    {
        cout << "Сначала создайте группу: m <size>"<< endl;
        return;
    }

    if (cur >= size)
    {
        cout << "Вектор заполнен"<<endl;
        return;
    }

    cout << "1. Print"<<endl;
    cout << "2. Magazin"<<endl;
    cout << "Выберите class: ";

    int choice = 0;
    cin >> choice;

    Object* object = nullptr;
    if (choice == 1)
    {
        object = new Print;
    }
    else if (choice == 2)
    {
        object = new Magazin;
    }
    else
    {
        cout << "Неправильный номер класса"<<endl;
        return;
    }

    object->Input();
    beg[cur] = object;
    ++cur;
}

void Vector::Del()
{
    if (cur == 0)
    {
        cout << "Вектор пуст"<<endl;
        return;
    }

    --cur;
    delete beg[cur];
    beg[cur] = nullptr;
}

void Vector::Show() const
{
    if (cur == 0)
    {
        cout << "Вектор пуст"<<endl;
        return;
    }

    cout << "Размер вектора: " << size << endl;
    cout << "Количество элементов: " << cur << endl;

    for (int i = 0; i < cur; ++i)
    {
        cout << endl<<"Элемент номер" << i + 1 << endl;
        beg[i]->Show();
    }
}

void Vector::HandleEvent(const TEvent& event)
{
    if (event.what == evMessage)
    {
        for (int i = 0; i < cur; ++i)
        {
            beg[i]->HandleEvent(event);
        }
    }
}
