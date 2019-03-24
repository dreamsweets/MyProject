#pragma once
#include "Precompiled.h"
#include "System.h"

class InputSystem :
	public System
{
public:
	InputSystem();
	~InputSystem();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	bool IsKeyDown(unsigned int);

private:

	bool keys[256];
};

