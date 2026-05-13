#pragma once

#include <iostream>

using namespace std;

class Time{
public:
    Time();
    Time(int minutes, int seconds);
    Time(const Time& other);

    int get_min() const;
    int get_sec() const;
    long long total_seconds() const;

    void set_min(int minutes);
    void set_sec(int seconds);

    Time& operator=(const Time& other);
    Time operator+(const Time& other) const;
    Time operator/(int value) const;
    Time operator*(const Time& other) const;

    bool operator<(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;

    static Time from_seconds(long long seconds);

    friend istream& operator>>(istream& in, Time& value);
    friend ostream& operator<<(ostream& out, const Time& value);

private:
    int min;
    int sec;

    void normalize();
};
