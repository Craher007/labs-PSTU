#include "Time.h"

#include <iomanip>

using namespace std;

Time::Time() : min(0), sec(0){}

Time::Time(int minutes, int seconds) : min(minutes), sec(seconds){ normalize();}

Time::Time(const Time& other) : min(other.min), sec(other.sec){}

int Time::get_min() const{ return min; }

int Time::get_sec() const{ return sec; }

long long Time::total_seconds() const{
    return static_cast<long long>(min) * 60 + sec;
}

void Time::set_min(int minutes){
    min = minutes;
    normalize();
}

void Time::set_sec(int seconds){
    sec = seconds;
    normalize();
}

Time& Time::operator=(const Time& other){
    if (this != &other){
        min = other.min;
        sec = other.sec;
    }
    return *this;
}

Time Time::operator+(const Time& other) const{
    return from_seconds(total_seconds() + other.total_seconds());
}

Time Time::operator/(int value) const{
    if (value == 0) return Time();
    return from_seconds(total_seconds() / value);
}

Time Time::operator*(const Time& other) const{
    return from_seconds(total_seconds() * other.total_seconds());
}

bool Time::operator<(const Time& other) const{
    return total_seconds() < other.total_seconds();
}

bool Time::operator>(const Time& other) const{
    return total_seconds() > other.total_seconds();
}

bool Time::operator==(const Time& other) const{
    return total_seconds() == other.total_seconds();
}

bool Time::operator!=(const Time& other) const{
    return !(*this == other);
}

Time Time::from_seconds(long long seconds){
    if (seconds < 0) seconds = 0;
    return Time(static_cast<int>(seconds / 60), static_cast<int>(seconds % 60));
}

void Time::normalize(){
    long long total = total_seconds();
    if (total < 0) total = 0;
    min = static_cast<int>(total / 60);
    sec = static_cast<int>(total % 60);
}

istream& operator>>(istream& in, Time& value){
    cout << "минуты: ";
    in >> value.min;
    cout << "секунды: ";
    in >> value.sec;
    value.normalize();
    return in;
}

ostream& operator<<(ostream& out, const Time& value){
    out << value.min << ':' << setw(2) << setfill('0') << value.sec << setfill(' ');
    return out;
}
