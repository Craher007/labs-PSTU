#include "ConsoleUI.h"
#include "DateUtils.h"
#include "Report.h"
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

ConsoleUI::ConsoleUI(Database& database) : database(database) {
}

string ConsoleUI::readLine(const string& message) {
    string value;
    cout << message;
    getline(cin, value);
    return value;
}

string ConsoleUI::trim(const string& value) const {
    size_t start = value.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }

    size_t end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

string ConsoleUI::readRequiredLine(const string& message) {
    while (true) {
        string value = trim(readLine(message));

        if (!value.empty()) {
            return value;
        }

        cout << "Поле не может быть пустым. Введите значение.\n";
    }
}

bool ConsoleUI::isPieceUnit(const string& unit) const {
    string cleanUnit = trim(unit);
    return cleanUnit == "шт" || cleanUnit == "ШТ" || cleanUnit == "Шт"
           || cleanUnit == "штук" || cleanUnit == "Штук"
           || cleanUnit == "штука" || cleanUnit == "Штука"
           || cleanUnit == "штуки" || cleanUnit == "Штуки";
}

bool ConsoleUI::isWholeNumber(double value) const {
    return fabs(value - round(value)) < 0.0001;
}

int ConsoleUI::readInt(const string& message, int minValue) {
    while (true) {
        string text = readLine(message);
        stringstream stream(text);
        int value = 0;
        stream >> value;
        stream >> ws;

        if (stream && stream.eof() && value >= minValue) {
            return value;
        }

        cout << "Введите целое число не меньше " << minValue << ".\n";
    }
}

int ConsoleUI::readChoice(const string& message, int minValue, int maxValue) {
    while (true) {
        int value = readInt(message, minValue);

        if (value <= maxValue) {
            return value;
        }

        cout << "Введите число от " << minValue << " до " << maxValue << ".\n";
    }
}

double ConsoleUI::readDouble(const string& message, double minValue) {
    while (true) {
        string text = readLine(message);
        for (char& symbol : text) {
            if (symbol == ',') {
                symbol = '.';
            }
        }

        stringstream stream(text);
        double value = 0;
        stream >> value;
        stream >> ws;

        if (stream && stream.eof() && value >= minValue) {
            return value;
        }

        cout << "Введите число не меньше " << formatQuantity(minValue) << ".\n";
    }
}

double ConsoleUI::readQuantity(const string& message, double minValue, const string& unit) {
    while (true) {
        double value = readDouble(message, minValue);

        if (!isPieceUnit(unit) || isWholeNumber(value)) {
            return value;
        }

        cout << "Для единицы измерения \"шт\" нужно ввести целое число.\n";
    }
}

Date ConsoleUI::readDate(const string& message) {
    while (true) {
        string text = readLine(message);
        Date date;

        if (parseDate(text, date)) {
            return date;
        }

        cout << "Введите дату в формате ДД.ММ.ГГГГ.\n";
    }
}

void ConsoleUI::showMainMenu() {
    cout << "\n========== Меню ==========\n";
    cout << "1. Рассчитать сроки закупки\n";
    cout << "2. Добавить изделие/деталь/материал\n";
    cout << "3. Изменить запись\n";
    cout << "4. Показать базу\n";
    cout << "5. Удалить запись\n";
    cout << "6. Сохранить базу\n";
    cout << "0. Выход\n";
}

void ConsoleUI::run() {
    while (true) {
        showMainMenu();
        int choice = readChoice("Выберите действие: ", 0, 6);

        if (choice == 1) {
            calculatePlan();
        } else if (choice == 2) {
            addItem();
        } else if (choice == 3) {
            editItem();
        } else if (choice == 4) {
            listItems();
        } else if (choice == 5) {
            deleteItem();
        } else if (choice == 6) {
            if (database.save()) {
                cout << "База сохранена в файл: " << database.getFileName() << "\n";
            } else {
                cout << "Не удалось сохранить базу.\n";
            }
        } else if (choice == 0) {
            database.save();
            cout << "Работа завершена.\n";
            return;
        } else {
            cout << "Такого пункта нет.\n";
        }
    }
}

void ConsoleUI::printItem(const Item& item) const {
    cout << "\nНазвание: " << item.name << "\n";
    cout << "Единица измерения: " << item.unit << "\n";

    if (item.type == MANUFACTURED) {
        cout << "Тип: изготовление\n";
        cout << "Дней на цикл: " << item.days << "\n";
        cout << "Можно изготовить одновременно: " << formatQuantity(item.capacity) << " " << item.unit << "\n";

        if (item.components.empty()) {
            cout << "Состав: не указан\n";
        } else {
            cout << "Состав на 1 " << item.unit << ":\n";
            for (const Component& component : item.components) {
                cout << "- " << component.name << ": " << formatQuantity(component.quantity)
                     << " " << component.unit << "\n";
            }
        }
    } else {
        cout << "Тип: закупка\n";
        cout << "Дней на поставку: " << item.days << "\n";
        if (item.capacity > 0) {
            cout << "Лимит одной поставки: " << formatQuantity(item.capacity) << " " << item.unit << "\n";
        } else {
            cout << "Лимит одной поставки: без ограничения\n";
        }
    }
}

void ConsoleUI::listItems() {
    const vector<Item>& items = database.getItems();

    if (items.empty()) {
        cout << "База пока пустая.\n";
        return;
    }

    for (const Item& item : items) {
        printItem(item);
    }
}

Item ConsoleUI::createItem(const string& forcedName, const string& forcedUnit) {
    Item item;

    if (forcedName.empty()) {
        item.name = readRequiredLine("Название: ");
    } else {
        item.name = trim(forcedName);
        cout << "\nСоздание записи: " << item.name << "\n";
    }

    if (forcedUnit.empty()) {
        item.unit = readRequiredLine("Единица измерения (шт, м, м2 и т.д.): ");
    } else {
        item.unit = trim(forcedUnit);
        cout << "Единица измерения: " << item.unit << "\n";
    }

    cout << "Тип записи:\n";
    cout << "1. Изготавливается\n";
    cout << "2. Закупается\n";
    int type = readChoice("Выберите тип: ", 1, 2);
    item.type = type == 2 ? PURCHASED : MANUFACTURED;

    if (item.type == MANUFACTURED) {
        item.days = readInt("Сколько дней занимает один цикл изготовления: ", 0);
        item.capacity = readQuantity("Сколько можно изготовить одновременно: ", 0.0001, item.unit);

        int componentCount = readInt("Сколько компонентов нужно для 1 единицы изделия: ", 0);
        for (int i = 0; i < componentCount; i++) {
            cout << "\nКомпонент " << i + 1 << " для изделия: " << item.name << "\n";
            Component component;
            component.name = readRequiredLine("Название компонента: ");
            component.unit = readRequiredLine("Единица измерения компонента: ");
            component.quantity = readQuantity("Количество на 1 единицу изделия: ", 0.0001, component.unit);
            item.components.push_back(component);

            if (database.findItem(component.name) == nullptr) {
                string answer = readLine("Такой записи еще нет. Создать ее сейчас? (да/нет): ");
                if (answer == "да" || answer == "Да" || answer == "yes" || answer == "y") {
                    Item child = createItem(component.name, component.unit);
                    database.addOrUpdateItem(child);
                }
            }
        }
    } else {
        item.days = readInt("Сколько дней идет поставка: ", 0);
        item.capacity = readQuantity("Сколько можно заказать за одну поставку (0 - без ограничения): ", 0, item.unit);
    }

    return item;
}

void ConsoleUI::addItem() {
    Item item = createItem();
    bool added = database.addOrUpdateItem(item);

    if (added) {
        cout << "Запись добавлена.\n";
    } else {
        cout << "Запись с таким названием была обновлена.\n";
    }

    database.save();
}

void ConsoleUI::editItem() {
    string name = readRequiredLine("Введите название записи для изменения: ");
    Item* item = database.findItem(name);

    if (item == nullptr) {
        cout << "Запись не найдена.\n";
        return;
    }

    printItem(*item);
    cout << "\n1. Полностью перезаписать запись\n";
    cout << "2. Переименовать запись\n";
    cout << "0. Отмена\n";
    int choice = readChoice("Выберите действие: ", 0, 2);

    if (choice == 1) {
        Item updated = createItem(name);
        database.addOrUpdateItem(updated);
        database.save();
        cout << "Запись обновлена.\n";
    } else if (choice == 2) {
        string newName = readRequiredLine("Новое название: ");
        if (database.renameItem(name, newName)) {
            database.save();
            cout << "Название изменено.\n";
        } else {
            cout << "Не удалось переименовать запись. Возможно, новое название уже занято.\n";
        }
    }
}

void ConsoleUI::deleteItem() {
    string name = readRequiredLine("Введите название записи для удаления: ");

    if (database.removeItem(name)) {
        database.save();
        cout << "Запись удалена. Также удалены ссылки на нее из составов других изделий.\n";
    } else {
        cout << "Запись не найдена.\n";
    }
}

void ConsoleUI::calculatePlan() {
    string productName = readRequiredLine("Название готового продукта: ");

    if (database.findItem(productName) == nullptr) {
        string answer = readLine("Такой записи нет. Создать ее сейчас? (да/нет): ");
        if (answer == "да" || answer == "Да" || answer == "yes" || answer == "y") {
            Item item = createItem(productName);
            database.addOrUpdateItem(item);
            database.save();
        } else {
            cout << "Расчет невозможен без записи в базе.\n";
            return;
        }
    }

    const Item* product = database.findItem(productName);
    string productUnit = product == nullptr ? "" : product->unit;
    double quantity = readQuantity("Количество готового продукта: ", 0.0001, productUnit);
    Date requestDate = readDate("Дата обращения (ДД.ММ.ГГГГ): ");
    Date dueDate = readDate("Дата, к которой нужен готовый продукт (ДД.ММ.ГГГГ): ");
    int reserveDays = readInt("Запас для оптимальной закупки в днях (обычно 10-15): ", 0);

    Planner planner(database);
    PlanResult result = planner.makePlan(productName, quantity, dueDate, reserveDays);
    printPlanResult(result, requestDate);
}
