#ifndef REPORT_H
#define REPORT_H

#include "Planner.h"
#include <string>
#include <vector>

using namespace std;

string formatQuantity(double value);
vector<StockNeed> aggregateStockNeedsForReport(const vector<StockNeed>& source);
vector<PurchaseNeed> aggregatePurchasesForReport(const vector<PurchaseNeed>& source, int reserveDays);
void printPlanResult(const PlanResult& result, const Date& requestDate);

#endif
