#pragma once
#include <iostream>
using namespace std;

class Time
{
public:
	Time(void) { min = sec = 0; }
	Time(int M, int S) { min = M; sec = S; }
	Time(const Time&);
	Time& operator=(const Time&);
	Time operator*(const Time&) const;

	friend ostream& operator<< (ostream& out, const Time&);
	friend istream& operator>> (istream& in, Time&);
public:
	virtual ~Time(void) {};
private:
	int min, sec;
};
