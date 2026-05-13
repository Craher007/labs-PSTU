#pragma once
#include <iostream>

using namespace std;

class Time
{
	int min;
	int sec;
public:
	Time() { min = 0; sec = 0; };
	Time(int m, int s) { min = m; sec = s; }
	Time(const Time& t) { min = t.min; sec = t.sec; }
	~Time() {};

	Time& operator=(const Time&);
	bool operator==(const Time&);
	bool operator!=(const Time&);
	bool operator>(const Time&);
	bool operator<(const Time&);
	Time operator-(const Time&);
	Time operator+(const Time&);
	
	friend istream& operator>>(istream& in, Time& t);
	friend ostream& operator<<(ostream& out, const Time& t);
};