#include "Header.h"
#include <iostream>

int Time::calculation() { return(hour * 60 + minute); }
void Time::Init(int a, int b) {
	hour = a;
	minute = b;
}