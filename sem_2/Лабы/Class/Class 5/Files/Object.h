#pragma once

using namespace std;

class object{
public:
	object(void) {}
	virtual ~object(void) {}
	virtual void show() = 0;
};