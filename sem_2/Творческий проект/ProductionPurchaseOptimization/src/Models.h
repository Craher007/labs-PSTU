#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

using namespace std;

// Простая дата без времени.
struct Date {
    int day = 1;
    int month = 1;
    int year = 2026;
};

// Тип записи: деталь/изделие изготавливается или закупается.
enum ItemType {
    MANUFACTURED = 1,
    PURCHASED = 2
};

// Компонент, который нужен для изготовления одной единицы родительского изделия.
struct Component {
    string name;
    double quantity = 0;
    string unit;
};

// Изделие, деталь или закупаемый материал.
struct Item {
    string name;
    string unit;
    ItemType type = MANUFACTURED;

    // Для производства: сколько дней занимает один цикл изготовления.
    // Для закупки: сколько дней идет одна поставка.
    int days = 1;

    // Для производства: сколько единиц можно делать одновременно.
    // Для закупки: сколько единиц можно заказать за одну поставку.
    // Если значение <= 0, программа считает, что ограничения нет.
    double capacity = 1;

    vector<Component> components;
};

#endif
