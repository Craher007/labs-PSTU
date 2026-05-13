#pragma once

#include <iomanip>
#include <iostream>

class Time{
private:
    int min;
    int sec;

    void normalize(){
        long long total = total_seconds();

        if (total < 0) total = 0;

        min = static_cast<int>(total / 60);
        sec = static_cast<int>(total % 60);
    }

public:
    Time() : min(0), sec(0) {}

    Time(int minutes, int seconds) : min(minutes), sec(seconds){ normalize(); }

    Time(const Time& other) : min(other.min), sec(other.sec) {}

    int get_min() const{ return min; }

    int get_sec() const{ return sec; }

    long long total_seconds() const{ return static_cast<long long>(min) * 60 + sec; }

    void set_min(int minutes){ min = minutes; normalize(); }

    void set_sec(int seconds){ sec = seconds; normalize(); }

    Time& operator=(const Time& other){
        if (this != &other){
            min = other.min;
            sec = other.sec;
        }

        return *this;
    }

    Time operator+(const Time& other) const{
        return from_seconds(total_seconds() + other.total_seconds());
    }

    Time operator/(int value) const{
        if (value == 0) return Time();

        return from_seconds(total_seconds() / value);
    }

    Time operator*(const Time& other) const{
        return from_seconds(total_seconds() * other.total_seconds());
    }

    bool operator<(const Time& other) const{ return total_seconds() < other.total_seconds(); }

    bool operator>(const Time& other) const{ return total_seconds() > other.total_seconds(); }

    bool operator==(const Time& other) const{ return total_seconds() == other.total_seconds(); }

    bool operator!=(const Time& other) const{ return !(*this == other); }

    static Time from_seconds(long long seconds){
        if (seconds < 0) seconds = 0;

        return Time(static_cast<int>(seconds / 60), static_cast<int>(seconds % 60));
    }

    friend istream& operator>>(istream& in, Time& value){
        cout << "минуты: ";
        in >> value.min;
        cout << "секунды: ";
        in >> value.sec;

        value.normalize();
        return in;
    }

    friend ostream& operator<<(ostream& out, const Time& value){
        out << value.min << ':' << setw(2) << setfill('0') << value.sec << setfill(' ');
        return out;
    }
};
