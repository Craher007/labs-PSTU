#pragma once

#include "Print.h"

class Magazin : public Print
{
public:
    Magazin();
    Magazin(const std::string& name, const std::string& author, int pages);
    Magazin(const Magazin& other);
    ~Magazin() override = default;

    void Show() const override;
    void Input() override;

    int GetPages() const { return pages; }
    void SetPages(int pages) { this->pages = pages; }

    Magazin& operator=(const Magazin& other);

protected:
    int pages;
};
