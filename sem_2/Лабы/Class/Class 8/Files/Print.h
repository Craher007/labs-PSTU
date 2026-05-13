#pragma once

#include "Object.h"

#include <string>
using namespace std;

class Print : public Object
{
public:
    Print();
    Print(const string& name, const string& author);
    Print(const Print& other);
    ~Print() override = default;

    void Show() const override;
    void Input() override;
    void HandleEvent(const TEvent& event) override;

    string GetName() const { return name; }
    string GetAuthor() const { return author; }
    void SetName(const string& name) { this->name = name; }
    void SetAuthor(const string& author) { this->author = author; }

    Print& operator=(const Print& other);

protected:
    string name;
    string author;
};
