#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "ConsoleUI.h"
#include "Database.h"
#include "Planner.h"
#include "Report.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

static void addDemoData(Database& database) {
    Item beam;
    beam.name = "Брус";
    beam.unit = "м";
    beam.type = PURCHASED;
    beam.days = 10;
    beam.capacity = 1000;
    database.addOrUpdateItem(beam);

    Item plywood;
    plywood.name = "Фанера";
    plywood.unit = "м2";
    plywood.type = PURCHASED;
    plywood.days = 10;
    plywood.capacity = 1000;
    database.addOrUpdateItem(plywood);

    Item bolts;
    bolts.name = "Болты";
    bolts.unit = "шт";
    bolts.type = PURCHASED;
    bolts.days = 10;
    bolts.capacity = 0;
    database.addOrUpdateItem(bolts);

    Item leg;
    leg.name = "Ножка";
    leg.unit = "шт";
    leg.type = MANUFACTURED;
    leg.days = 1;
    leg.capacity = 12;
    leg.components.push_back({"Брус", 1, "м"});
    database.addOrUpdateItem(leg);

    Item back;
    back.name = "Спинка";
    back.unit = "шт";
    back.type = MANUFACTURED;
    back.days = 1;
    back.capacity = 3;
    back.components.push_back({"Фанера", 1, "м2"});
    database.addOrUpdateItem(back);

    Item chair;
    chair.name = "Стул";
    chair.unit = "шт";
    chair.type = MANUFACTURED;
    chair.days = 1;
    chair.capacity = 3;
    chair.components.push_back({"Ножка", 4, "шт"});
    chair.components.push_back({"Спинка", 1, "шт"});
    chair.components.push_back({"Болты", 4, "шт"});
    database.addOrUpdateItem(chair);
}

static void runDemo() {
    Database database("demo_database.txt");
    addDemoData(database);

    Date requestDate;
    parseDate("01.01.2026", requestDate);

    Date dueDate;
    parseDate("30.01.2026", dueDate);

    Planner planner(database);

    cout << "Демонстрационный расчет по примеру из задания.\n";
    PlanResult result = planner.makePlan("Стул", 5, dueDate, 10);
    printPlanResult(result, requestDate);

    cout << "\nПримечание: в примере задания указано 5 м бруса для 20 ножек.\n";
    cout << "Если ввести 1 м бруса на одну ножку, программа правильно считает 20 м.\n";
    cout << "Чтобы получить 5 м, нужно указать 0.25 м бруса на одну ножку.\n";
}

static const PurchaseNeed* findPurchase(const PlanResult& result, const string& name) {
    for (const PurchaseNeed& purchase : result.purchases) {
        if (purchase.name == name) {
            return &purchase;
        }
    }

    return nullptr;
}

static bool checkDate(const string& testName, const string& fieldName,
                      const Date& actual, const string& expectedText) {
    Date expected;
    parseDate(expectedText, expected);

    if (isDateEqual(actual, expected)) {
        cout << "[OK] " << testName << ": " << fieldName << "\n";
        return true;
    }

    cout << "[ОШИБКА] " << testName << ": " << fieldName
         << ", ожидалось " << expectedText
         << ", получено " << formatDate(actual) << "\n";
    return false;
}

static bool checkNumber(const string& testName, const string& fieldName,
                        double actual, double expected) {
    if (fabs(actual - expected) < 0.0001) {
        cout << "[OK] " << testName << ": " << fieldName << "\n";
        return true;
    }

    cout << "[ОШИБКА] " << testName << ": " << fieldName
         << ", ожидалось " << formatQuantity(expected)
         << ", получено " << formatQuantity(actual) << "\n";
    return false;
}

static bool checkBool(const string& testName, const string& fieldName,
                      bool actual, bool expected) {
    if (actual == expected) {
        cout << "[OK] " << testName << ": " << fieldName << "\n";
        return true;
    }

    cout << "[ОШИБКА] " << testName << ": " << fieldName << "\n";
    return false;
}

static bool runTests() {
    bool success = true;

    Date testDate;
    success = checkBool("Даты", "правильная дата принимается",
                        parseDate("29.02.2028", testDate), true) && success;
    success = checkBool("Даты", "неправильный день отклоняется",
                        parseDate("32.01.2026", testDate), false) && success;
    success = checkBool("Даты", "неправильный високосный день отклоняется",
                        parseDate("29.02.2026", testDate), false) && success;
    success = checkBool("Даты", "неправильный разделитель отклоняется",
                        parseDate("01-01-2026", testDate), false) && success;
    success = checkBool("Даты", "лишний текст отклоняется",
                        parseDate("01.01.2026abc", testDate), false) && success;

    {
        ofstream badDatabaseFile("test_bad_database.txt");
        badDatabaseFile << "ITEM|Тестовая деталь|шт|1|1|10\n";
        badDatabaseFile << "COMP|Тестовая деталь||1|шт\n";
        badDatabaseFile << "COMP|Тестовая деталь|Нормальный компонент|2|шт\n";
        badDatabaseFile.close();

        Database badDatabase("test_bad_database.txt");
        badDatabase.load();

        const Item* badItem = badDatabase.findItem("Тестовая деталь");
        success = checkBool("База", "запись с пробелами в названии найдена",
                            badItem != nullptr, true) && success;

        if (badItem != nullptr) {
            success = checkNumber("База", "пустой компонент пропущен",
                                  badItem->components.size(), 1) && success;
            success = checkBool("База", "нормальный компонент сохранен",
                                badItem->components[0].name == "Нормальный компонент", true) && success;
        }
    }

    Database chairDatabase("test_chair_database.txt");
    addDemoData(chairDatabase);

    Date chairDueDate;
    parseDate("30.01.2026", chairDueDate);

    Planner chairPlanner(chairDatabase);
    PlanResult chairResult = chairPlanner.makePlan("Стул", 5, chairDueDate, 10);

    const PurchaseNeed* beam = findPurchase(chairResult, "Брус");
    const PurchaseNeed* plywood = findPurchase(chairResult, "Фанера");
    const PurchaseNeed* bolts = findPurchase(chairResult, "Болты");

    if (beam == nullptr || plywood == nullptr || bolts == nullptr) {
        cout << "[ОШИБКА] Пример со стулом: не найдены все закупаемые материалы.\n";
        success = false;
    } else {
        success = checkNumber("Стул", "количество бруса", beam->quantity, 20) && success;
        success = checkDate("Стул", "брус нужен на складе", beam->needDate, "26.01.2026") && success;
        success = checkDate("Стул", "крайний срок бруса", beam->deadlineDate, "16.01.2026") && success;
        success = checkDate("Стул", "оптимальный срок бруса", beam->optimalDate, "06.01.2026") && success;

        success = checkNumber("Стул", "количество фанеры", plywood->quantity, 5) && success;
        success = checkDate("Стул", "крайний срок фанеры", plywood->deadlineDate, "16.01.2026") && success;

        success = checkNumber("Стул", "количество болтов", bolts->quantity, 20) && success;
        success = checkDate("Стул", "болты нужны на складе", bolts->needDate, "28.01.2026") && success;
        success = checkDate("Стул", "крайний срок болтов", bolts->deadlineDate, "18.01.2026") && success;
    }

    Database duplicateDatabase("test_duplicate_database.txt");
    Item duplicateBeam;
    duplicateBeam.name = "Брус";
    duplicateBeam.unit = "м";
    duplicateBeam.type = PURCHASED;
    duplicateBeam.days = 10;
    duplicateBeam.capacity = 1000;
    duplicateDatabase.addOrUpdateItem(duplicateBeam);

    Item duplicateProduct;
    duplicateProduct.name = "Тестовое изделие";
    duplicateProduct.unit = "шт";
    duplicateProduct.type = MANUFACTURED;
    duplicateProduct.days = 1;
    duplicateProduct.capacity = 10;
    duplicateProduct.components.push_back({"Брус", 10, "м"});
    duplicateProduct.components.push_back({"Брус", 5, "м"});
    duplicateDatabase.addOrUpdateItem(duplicateProduct);

    Date duplicateDueDate;
    Date duplicateRequestDate;
    parseDate("20.02.2026", duplicateDueDate);
    parseDate("01.02.2026", duplicateRequestDate);

    Planner duplicatePlanner(duplicateDatabase);
    PlanResult duplicateResult = duplicatePlanner.makePlan("Тестовое изделие", 1, duplicateDueDate, 10);

    stringstream reportOutput;
    streambuf* oldBuffer = cout.rdbuf(reportOutput.rdbuf());
    printPlanResult(duplicateResult, duplicateRequestDate);
    cout.rdbuf(oldBuffer);

    string reportText = reportOutput.str();
    success = checkBool("Закупки", "одинаковый материал объединяется",
                        reportText.find("Брус - 15 м") != string::npos, true) && success;

    Date lateDueDate;
    Date lateRequestDate;
    parseDate("20.02.2026", lateDueDate);
    parseDate("15.02.2026", lateRequestDate);

    Planner latePlanner(duplicateDatabase);
    PlanResult lateResult = latePlanner.makePlan("Тестовое изделие", 1, lateDueDate, 10);

    stringstream lateReportOutput;
    oldBuffer = cout.rdbuf(lateReportOutput.rdbuf());
    printPlanResult(lateResult, lateRequestDate);
    cout.rdbuf(oldBuffer);

    string lateReportText = lateReportOutput.str();
    success = checkBool("Сроки", "сообщение об опоздании выводится",
                        lateReportText.find("Сроки не выполняются") != string::npos, true) && success;
    success = checkBool("Сроки", "лучшая дата готовности рассчитана",
                        lateReportText.find("26.02.2026") != string::npos, true) && success;
    success = checkBool("Сроки", "дата готовности с запасом рассчитана",
                        lateReportText.find("08.03.2026") != string::npos, true) && success;

    Database capacityDatabase("test_capacity_database.txt");
    Item material;
    material.name = "Материал";
    material.unit = "кг";
    material.type = PURCHASED;
    material.days = 5;
    material.capacity = 100;
    capacityDatabase.addOrUpdateItem(material);

    Item body;
    body.name = "Корпус";
    body.unit = "шт";
    body.type = MANUFACTURED;
    body.days = 2;
    body.capacity = 2;
    body.components.push_back({"Материал", 250, "кг"});
    capacityDatabase.addOrUpdateItem(body);

    Date bodyDueDate;
    parseDate("20.02.2026", bodyDueDate);

    Planner capacityPlanner(capacityDatabase);
    PlanResult capacityResult = capacityPlanner.makePlan("Корпус", 1, bodyDueDate, 15);
    const PurchaseNeed* materialPurchase = findPurchase(capacityResult, "Материал");

    if (materialPurchase == nullptr) {
        cout << "[ОШИБКА] Проверка партий: материал не найден.\n";
        success = false;
    } else {
        success = checkNumber("Партии", "количество материала", materialPurchase->quantity, 250) && success;
        success = checkNumber("Партии", "число партий", materialPurchase->batches, 3) && success;
        success = checkNumber("Партии", "срок поставки", materialPurchase->deliveryDays, 15) && success;
        success = checkDate("Партии", "крайний срок", materialPurchase->deadlineDate, "03.02.2026") && success;
        success = checkDate("Партии", "оптимальный срок", materialPurchase->optimalDate, "19.01.2026") && success;
    }

    if (success) {
        cout << "\nВсе тесты пройдены.\n";
    } else {
        cout << "\nЕсть ошибки в тестах.\n";
    }

    return success;
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    if (argc > 1 && string(argv[1]) == "--demo") {
        runDemo();
        return 0;
    }

    if (argc > 1 && string(argv[1]) == "--tests") {
        return runTests() ? 0 : 1;
    }

    Database database("database.txt");
    database.load();

    cout << "Оптимизация производственного закупа\n";
    cout << "База данных: " << database.getFileName() << "\n";

    ConsoleUI ui(database);
    ui.run();

    return 0;
}
