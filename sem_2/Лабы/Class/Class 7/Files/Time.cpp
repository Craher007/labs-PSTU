#include "Time.h"

Time::Time(const Time& t)
{
	min = t.min;
	sec = t.sec;
}
Time& Time::operator =(const Time& t)
{
	min = t.min;
	sec = t.sec;
	return*this;
}
Time Time::operator*(const Time& t) const
{
	int left = min * 60 + sec;
	int right = t.min * 60 + t.sec;
	int result = left * right;
	return Time(result / 60, result % 60);
}
ostream& operator<<(ostream& out, const Time& t)
{
	out << t.min << ":" << t.sec;
	return out;
}
istream& operator>>(istream& in, Time& t)
{
	cout << endl << "min: "; in >> t.min;
	cout << endl << "sec: "; in >> t.sec;
	return in;
}
