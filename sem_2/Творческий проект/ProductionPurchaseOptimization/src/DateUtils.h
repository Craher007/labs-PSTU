#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include "Models.h"
#include <string>

using namespace std;

bool parseDate(const string& text, Date& date);
string formatDate(const Date& date);
Date addDays(const Date& date, int days);
int daysBetween(const Date& startDate, const Date& endDate);
int compareDates(const Date& left, const Date& right);
bool isDateBefore(const Date& left, const Date& right);
bool isDateEqual(const Date& left, const Date& right);

#endif
