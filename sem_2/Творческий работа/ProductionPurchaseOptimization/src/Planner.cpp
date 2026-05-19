#include "Planner.h"
#include <cmath>
#include <sstream>

using namespace std;

Planner::Planner(const Database& database) : database(database) {
}

int Planner::calculateDuration(int days, double capacity, double quantity) const {
    // Один цикл занимает days дней. Если за цикл можно сделать/заказать capacity единиц,
    // то количество циклов равно quantity / capacity с округлением вверх.
    if (days < 0) {
        days = 0;
    }

    if (capacity <= 0) {
        return days;
    }

    int cycles = static_cast<int>(ceil(quantity / capacity));
    if (cycles < 1) {
        cycles = 1;
    }

    return cycles * days;
}

bool Planner::containsName(const vector<string>& path, const string& name) const {
    // path хранит текущую цепочку расчета. Так мы ловим циклы вида Стул -> Ножка -> Стул.
    for (const string& value : path) {
        if (value == name) {
            return true;
        }
    }

    return false;
}

void Planner::planItem(const string& name, double quantity, const Date& needDate,
                       int reserveDays, vector<string>& path, PlanResult& result) const {
    // Главная рекурсивная функция расчета.
    // Она идет от готового изделия вниз к деталям и закупаемым материалам.
    const Item* item = database.findItem(name);

    if (item == nullptr) {
        result.errors.push_back("В базе не найдена запись: " + name);
        return;
    }

    if (containsName(path, name)) {
        result.errors.push_back("В составе найден цикл. Проверьте запись: " + name);
        return;
    }

    path.push_back(name);

    if (item->type == PURCHASED) {
        // Закупка заканчивает ветку расчета: дальше компонентов нет,
        // поэтому считаем крайний и оптимальный срок заказа.
        int deliveryDays = calculateDuration(item->days, item->capacity, quantity);

        PurchaseNeed purchase;
        purchase.name = item->name;
        purchase.quantity = quantity;
        purchase.unit = item->unit;
        purchase.baseDeliveryDays = item->days;
        purchase.deliveryDays = deliveryDays;
        purchase.batchLimit = item->capacity;
        purchase.batches = item->capacity > 0 ? static_cast<int>(ceil(quantity / item->capacity)) : 1;
        purchase.needDate = needDate;
        purchase.deadlineDate = addDays(needDate, -deliveryDays);
        purchase.optimalDate = addDays(purchase.deadlineDate, -reserveDays);

        result.purchases.push_back(purchase);
        path.pop_back();
        return;
    }

    int productionDays = calculateDuration(item->days, item->capacity, quantity);
    Date componentsNeedDate = addDays(needDate, -productionDays);

    // Для изготавливаемой записи сначала вычисляем дату, когда ее компоненты должны быть на складе,
    // затем рекурсивно считаем каждый компонент.
    for (const Component& component : item->components) {
        if (component.name.empty()) {
            result.errors.push_back("У изделия есть компонент без названия: " + item->name);
            continue;
        }

        double componentQuantity = quantity * component.quantity;

        StockNeed stockNeed;
        stockNeed.name = component.name;
        stockNeed.quantity = componentQuantity;
        stockNeed.unit = component.unit;
        stockNeed.readyDate = componentsNeedDate;
        result.stockNeeds.push_back(stockNeed);

        planItem(component.name, componentQuantity, componentsNeedDate,
                 reserveDays, path, result);
    }

    path.pop_back();
}

PlanResult Planner::makePlan(const string& productName, double quantity,
                             const Date& dueDate, int reserveDays) const {
    // Внешняя точка входа в расчет: подготавливает результат и запускает рекурсию от готового изделия.
    PlanResult result;
    result.productName = productName;
    result.productQuantity = quantity;
    result.dueDate = dueDate;
    result.reserveDays = reserveDays;

    const Item* item = database.findItem(productName);
    if (item != nullptr) {
        result.productUnit = item->unit;
    }

    vector<string> path;
    planItem(productName, quantity, dueDate, reserveDays, path, result);

    return result;
}
