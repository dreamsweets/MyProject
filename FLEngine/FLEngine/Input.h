#pragma once
#include "System.h"

class Input :
	public System
{
public:
	Input();
	~Input();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	bool IsKeyDown(unsigned int);

private:
	bool keys[256];
};

