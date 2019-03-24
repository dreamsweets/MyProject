#include "InputSystem.h"


InputSystem::InputSystem()
{
	// Initialize all the keys to being released and not pressed.
	for (int i = 0; i<256; i++)
	{
		keys[i] = false;
	}
}


InputSystem::~InputSystem()
{
}

void InputSystem::KeyDown(unsigned int input)
{
	keys[input] = true;
	return;
}

void InputSystem::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	keys[input] = false;
	return;
}

bool InputSystem::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return keys[key];
}