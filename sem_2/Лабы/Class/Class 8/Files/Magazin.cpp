#include "Magazin.h"

#include <iostream>
using namespace std;

Magazin::Magazin(): Print(), pages(0){}

Magazin::Magazin(const string& name, const string& author, int pages): Print(name, author), pages(pages){}

Magazin::Magazin(const Magazin& other): Print(other), pages(other.pages){}

void Magazin::Show() const
{
    Print::Show();
    cout << "Страницы: " << pages << endl;
}

void Magazin::Input()
{
    Print::Input();

    cout << "Страницы: ";
    cin >> pages;
}

Magazin& Magazin::operator=(const Magazin& other)
{
    if (this != &other)
    {
        Print::operator=(other);
        pages = other.pages;
    }

    return *this;
}
