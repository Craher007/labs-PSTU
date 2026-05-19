#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include "Database.h"
#include "Planner.h"
#include <string>

using namespace std;

class ConsoleUI {
private:
    Database& database;

    string readLine(const string& message);
    string trim(const string& value) const;
    string readRequiredLine(const string& message);
    bool isPieceUnit(const string& unit) const;
    bool isWholeNumber(double value) const;
    int readInt(const string& message, int minValue);
    int readChoice(const string& message, int minValue, int maxValue);
    double readDouble(const string& message, double minValue);
    double readQuantity(const string& message, double minValue, const string& unit);
    Date readDate(const string& message);

    void showMainMenu();
    void listItems();
    void addItem();
    void editItem();
    void deleteItem();
    void calculatePlan();
    void printItem(const Item& item) const;
    Item createItem(const string& forcedName = "", const string& forcedUnit = "");

public:
    ConsoleUI(Database& database);
    void run();
};

#endif
