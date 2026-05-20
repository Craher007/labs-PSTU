#ifndef PLANNER_H
#define PLANNER_H

#include "Database.h"
#include "DateUtils.h"
#include <string>
#include <vector>

using namespace std;

struct StockNeed {
    string name;
    double quantity = 0;
    string unit;
    Date readyDate;
};

struct PurchaseNeed {
    string name;
    double quantity = 0;
    string unit;
    int baseDeliveryDays = 0;
    int deliveryDays = 0;
    double batchLimit = 0;
    int batches = 1;
    Date needDate;
    Date deadlineDate;
    Date optimalDate;
};

struct PlanResult {
    string productName;
    double productQuantity = 0;
    string productUnit;
    Date dueDate;
    int reserveDays = 10;
    vector<StockNeed> stockNeeds;
    vector<PurchaseNeed> purchases;
    vector<string> warnings;
    vector<string> errors;
};

class Planner {
private:
    const Database& database;

    int calculateDuration(int days, double capacity, double quantity) const;
    bool containsName(const vector<string>& path, const string& name) const;
    void planItem(const string& name, double quantity, const Date& needDate,
                  int reserveDays, vector<string>& path, PlanResult& result) const;

public:
    Planner(const Database& database);
    PlanResult makePlan(const string& productName, double quantity,
                        const Date& dueDate, int reserveDays) const;
};

#endif
