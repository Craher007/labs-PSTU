#pragma once
#include <iostream>

using namespace std;

class Time{
    int min;
    int sec;

    void normalize(){
        int total = min * 60 + sec;
        min = total / 60;
        sec = total % 60;
    }

public:
    Time() { min = 0; sec = 0; }
    Time(int m, int s){ min = m; sec = s; normalize(); }
    Time(const Time& t){ min = t.min; sec = t.sec; }

    ~Time() {}

    int get_min() const { return min; }
    int get_sec() const { return sec; }
    int to_seconds() const { return min * 60 + sec; }

    void set_min(int m){ min = m; normalize(); }
    void set_sec(int s){ sec = s; normalize(); }

    Time& operator=(const Time& t){
        if (this == &t) return *this;
        min = t.min;
        sec = t.sec;
        return *this;
    }

    bool operator<(const Time& t) const{ return to_seconds() < t.to_seconds(); }

    bool operator>(const Time& t) const{ return to_seconds() > t.to_seconds(); }

    bool operator==(const Time& t) const{ return min == t.min && sec == t.sec; }

    bool operator!=(const Time& t) const{ return !(*this == t); }

    Time operator+(const Time& t) const{
        int total = to_seconds() + t.to_seconds();
        return Time(total / 60, total % 60);
    }

    Time operator/(int n) const{
        int total = to_seconds() / n;
        return Time(total / 60, total % 60);
    }

    Time operator*(const Time& t) const{
        int total = to_seconds() * t.to_seconds();
        return Time(total / 60, total % 60);
    }

    friend istream& operator>>(istream& in, Time& t){
        cout << "Минуты: ";
        in >> t.min;
        cout << "Секунды: ";
        in >> t.sec;
        t.normalize();
        return in;
    }

    friend ostream& operator<<(ostream& out, const Time& t){
        out << t.min << " : " << t.sec;
        return out;
    }
};
