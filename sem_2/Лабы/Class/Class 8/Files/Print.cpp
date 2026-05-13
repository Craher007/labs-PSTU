#include "Print.h"

#include <iostream>
using namespace std;

Print::Print(): name(), author(){}

Print::Print(const string& name, const string& author): name(name), author(author){}

Print::Print(const Print& other): name(other.name), author(other.author){}

void Print::Show() const
{
    cout << "Имя: " << name << endl;
    cout << "Автор: " << author << endl;
}

void Print::Input()
{
    cout << "Имя: ";
    getline(cin >> ws, name);

    cout << "Автор: ";
    getline(cin >> ws, author);
}

void Print::HandleEvent(const TEvent& event)
{
    if (event.what == evMessage && event.command == cmGet)
    {
        cout << "Имя = " << name << '\n';
    }
}


Print& Print::operator=(const Print& other)
{
    if (this != &other)
    {
        name = other.name;
        author = other.author;
    }

    return *this;
}
