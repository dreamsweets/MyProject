#include "Input.h"
#include "stdafx.h"

Input::Input()
{
	// Initialize all the keys to being released and not pressed.
	for (int i = 0; i<256; i++)
	{
		keys[i] = false;
	}
}


Input::~Input()
{
}

void Input::KeyDown(unsigned int input)
{
	keys[input] = true;
	return;
}

void Input::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	keys[input] = false;
	return;
}

bool Input::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return keys[key];
}