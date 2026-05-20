#include "DateUtils.h"
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

static time_t dateToTime(const Date& date) {
    // Для операций с датами используем стандартный time_t.
    tm value = {};
    value.tm_mday = date.day;
    value.tm_mon = date.month - 1;
    value.tm_year = date.year - 1900;
    value.tm_hour = 12;
    value.tm_isdst = -1;
    return mktime(&value);
}

static Date timeToDate(time_t value) {
    tm* converted = localtime(&value);
    Date date;

    if (converted == nullptr) {
        return date;
    }

    date.day = converted->tm_mday;
    date.month = converted->tm_mon + 1;
    date.year = converted->tm_year + 1900;
    return date;
}

bool parseDate(const string& text, Date& date) {
    // Ожидаемый формат: ДД.ММ.ГГГГ. Функция возвращает false, если дата написана неверно.
    int day = 0;
    int month = 0;
    int year = 0;
    char dot1 = '\0';
    char dot2 = '\0';

    stringstream stream(text);
    stream >> day >> dot1 >> month >> dot2 >> year;

    if (!stream || dot1 != '.' || dot2 != '.') {
        return false;
    }

    stream >> ws;
    if (!stream.eof()) {
        return false;
    }

    if (year < 1900 || year > 2200 || month < 1 || month > 12 || day < 1 || day > 31) {
        return false;
    }

    Date candidate;
    candidate.day = day;
    candidate.month = month;
    candidate.year = year;

    time_t timeValue = dateToTime(candidate);
    if (timeValue == static_cast<time_t>(-1)) {
        return false;
    }

    Date normalized = timeToDate(timeValue);

    // Проверяем, что mktime не "исправил" неправильную дату вроде 32.01.2026.
    if (normalized.day != day || normalized.month != month || normalized.year != year) {
        return false;
    }

    date = candidate;
    return true;
}

string formatDate(const Date& date) {
    stringstream stream;
    stream << setfill('0') << setw(2) << date.day << "."
           << setfill('0') << setw(2) << date.month << "."
           << date.year;
    return stream.str();
}

Date addDays(const Date& date, int days) {
    // Прибавление дней используется и для сроков производства, и для сроков закупки.
    time_t timeValue = dateToTime(date);
    timeValue += static_cast<time_t>(days) * 24 * 60 * 60;
    return timeToDate(timeValue);
}

int daysBetween(const Date& startDate, const Date& endDate) {
    time_t startTime = dateToTime(startDate);
    time_t endTime = dateToTime(endDate);
    double seconds = difftime(endTime, startTime);
    return static_cast<int>(seconds / (24 * 60 * 60));
}

int compareDates(const Date& left, const Date& right) {
    // Единая функция сравнения нужна, чтобы не дублировать правила сравнения дат по проекту.
    time_t leftTime = dateToTime(left);
    time_t rightTime = dateToTime(right);

    if (leftTime < rightTime) {
        return -1;
    }

    if (leftTime > rightTime) {
        return 1;
    }

    return 0;
}

bool isDateBefore(const Date& left, const Date& right) {
    return compareDates(left, right) < 0;
}

bool isDateEqual(const Date& left, const Date& right) {
    return compareDates(left, right) == 0;
}
