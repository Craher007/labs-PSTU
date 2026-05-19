#include "Report.h"
#include "DateUtils.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

string formatQuantity(double value) {
    // Количество без дробной части выводим как целое, чтобы вместо 10.00 пользователь видел 10.
    stringstream stream;

    if (fabs(value - round(value)) < 0.0001) {
        stream << static_cast<long long>(round(value));
    } else {
        stream << fixed << setprecision(2) << value;
    }

    return stream.str();
}

static bool compareStockNeeds(const StockNeed& left, const StockNeed& right) {
    int dateCompare = compareDates(left.readyDate, right.readyDate);
    if (dateCompare != 0) {
        return dateCompare < 0;
    }

    return left.name < right.name;
}

static bool comparePurchases(const PurchaseNeed& left, const PurchaseNeed& right) {
    int dateCompare = compareDates(left.deadlineDate, right.deadlineDate);
    if (dateCompare != 0) {
        return dateCompare < 0;
    }

    return left.name < right.name;
}

static int calculateBatches(double quantity, double capacity) {
    // Считает количество партий: если лимита нет, считаем одной партией.
    if (capacity <= 0) {
        return 1;
    }

    int batches = static_cast<int>(ceil(quantity / capacity));
    if (batches < 1) {
        batches = 1;
    }

    return batches;
}

vector<StockNeed> aggregateStockNeedsForReport(const vector<StockNeed>& source) {
    // Объединяем одинаковые складские потребности, но только если совпали название, единица и дата.
    // Разные даты склада нельзя складывать в одну строку.
    vector<StockNeed> result;

    for (const StockNeed& need : source) {
        bool found = false;

        for (StockNeed& existing : result) {
            if (existing.name == need.name && existing.unit == need.unit
                && isDateEqual(existing.readyDate, need.readyDate)) {
                existing.quantity += need.quantity;
                found = true;
                break;
            }
        }

        if (!found) {
            result.push_back(need);
        }
    }

    return result;
}

vector<PurchaseNeed> aggregatePurchasesForReport(const vector<PurchaseNeed>& source, int reserveDays) {
    // Закупки одного материала объединяем, чтобы пользователь видел общий объем заказа.
    // После объединения пересчитываем количество партий и даты заказа.
    vector<PurchaseNeed> result;

    for (const PurchaseNeed& purchase : source) {
        bool found = false;

        for (PurchaseNeed& existing : result) {
            if (existing.name == purchase.name && existing.unit == purchase.unit) {
                existing.quantity += purchase.quantity;

                if (isDateBefore(purchase.needDate, existing.needDate)) {
                    existing.needDate = purchase.needDate;
                }

                existing.batches = calculateBatches(existing.quantity, existing.batchLimit);
                existing.deliveryDays = existing.batches * existing.baseDeliveryDays;
                existing.deadlineDate = addDays(existing.needDate, -existing.deliveryDays);
                existing.optimalDate = addDays(existing.deadlineDate, -reserveDays);
                found = true;
                break;
            }
        }

        if (!found) {
            result.push_back(purchase);
        }
    }

    return result;
}

void printPlanResult(const PlanResult& result, const Date& requestDate) {
    // Консольный вывод оставил как вспомогательный вариант(Для проверок изначально было консольное предложение)
    // Qt-окно использует те же структуры PlanResult, StockNeed и PurchaseNeed.
    cout << "\n========== Результат расчета ==========\n";
    cout << "Готовый продукт: " << result.productName << " - "
         << formatQuantity(result.productQuantity) << " " << result.productUnit << "\n";
    cout << "Нужен к дате: " << formatDate(result.dueDate) << "\n";
    cout << "Запас для оптимальной закупки: " << result.reserveDays << " дней\n";

    if (!result.errors.empty()) {
        cout << "\nОшибки:\n";
        for (const string& error : result.errors) {
            cout << "- " << error << "\n";
        }
        return;
    }

    if (!result.warnings.empty()) {
        cout << "\nПредупреждения:\n";
        for (const string& warning : result.warnings) {
            cout << "- " << warning << "\n";
        }
    }

    vector<StockNeed> stockNeeds = aggregateStockNeedsForReport(result.stockNeeds);
    vector<PurchaseNeed> purchases = aggregatePurchasesForReport(result.purchases, result.reserveDays);

    sort(stockNeeds.begin(), stockNeeds.end(), compareStockNeeds);
    sort(purchases.begin(), purchases.end(), comparePurchases);

    cout << "\nЧто должно быть на складе:\n";
    if (stockNeeds.empty()) {
        cout << "Дополнительные детали не требуются.\n";
    } else {
        for (const StockNeed& need : stockNeeds) {
            cout << "- " << formatDate(need.readyDate) << ": " << need.name
                 << " - " << formatQuantity(need.quantity) << " " << need.unit << "\n";

            if (isDateBefore(need.readyDate, requestDate)) {
                cout << "  Внимание: эта дата уже прошла относительно даты обращения.\n";
            }
        }
    }

    cout << "\nСроки закупки:\n";
    if (purchases.empty()) {
        cout << "Закупаемых материалов в составе нет.\n";
    } else {
        for (const PurchaseNeed& purchase : purchases) {
            cout << "- " << purchase.name << " - " << formatQuantity(purchase.quantity)
                 << " " << purchase.unit << "\n";
            cout << "  Должно быть на складе: " << formatDate(purchase.needDate) << "\n";
            cout << "  Крайний срок заказа: " << formatDate(purchase.deadlineDate) << "\n";
            cout << "  Оптимальный срок заказа: " << formatDate(purchase.optimalDate) << "\n";
            cout << "  Срок поставки с учетом ограничения: " << purchase.deliveryDays << " дней";
            if (purchase.batchLimit > 0) {
                cout << ", партий: " << purchase.batches;
            }
            cout << "\n";

            if (isDateBefore(purchase.deadlineDate, requestDate)) {
                cout << "  Внимание: крайний срок уже прошел относительно даты обращения.\n";
            } else if (isDateBefore(purchase.optimalDate, requestDate)) {
                cout << "  Внимание: оптимальный срок уже прошел, но крайний срок еще доступен.\n";
            }
        }
    }

    if (!purchases.empty()) {
        Date minOptimal = purchases[0].optimalDate;
        Date minDeadline = purchases[0].deadlineDate;
        int maxDelayDays = 0;

        for (const PurchaseNeed& purchase : purchases) {
            if (isDateBefore(purchase.optimalDate, minOptimal)) {
                minOptimal = purchase.optimalDate;
            }
            if (isDateBefore(purchase.deadlineDate, minDeadline)) {
                minDeadline = purchase.deadlineDate;
            }
            if (isDateBefore(purchase.deadlineDate, requestDate)) {
                int delayDays = daysBetween(purchase.deadlineDate, requestDate);
                if (delayDays > maxDelayDays) {
                    maxDelayDays = delayDays;
                }
            }
        }

        if (maxDelayDays > 0) {
            Date bestFinishDate = addDays(result.dueDate, maxDelayDays);
            Date finishDateWithReserve = addDays(bestFinishDate, result.reserveDays);

            cout << "\nИтог:\n";
            cout << "Сроки не выполняются:\n";
            cout << "Вы не успеваете по срокам, потому что крайний срок закупки уже прошел.\n";
            cout << "Если заказать товары прямо сейчас, изделие будет готово в лучшем случае к "
                 << formatDate(bestFinishDate) << " без учета задержек.\n";
            cout << "Самая поздняя дата готовности с учетом запаса "
                 << result.reserveDays << " дней: " << formatDate(finishDateWithReserve) << "\n";
        } else {
            cout << "\nИтог:\n";
            cout << "Самый ранний оптимальный срок закупки: " << formatDate(minOptimal) << "\n";
            cout << "Самый ранний крайний срок закупки: " << formatDate(minDeadline) << "\n";
        }
    }
}
