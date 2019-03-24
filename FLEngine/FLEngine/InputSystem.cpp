#include "InputSystem.h"


InputSystem::InputSystem()
{
	// Initialize all the keys to being released and not pressed.
	for (int i = 0; i< 256; i++)
	{
		keys[i] = false;
	}
}


InputSystem::~InputSystem()
{
}

void InputSystem::KeyDown(unsigned int input)
{
	/*추가 : 배열의 크기보다 크면 오버플로나기 때문에 예외처리 해줘야한다.*/
	if (input >= 256) return;
	keys[input] = true;
	/*어차피 빠져나가기 때문에 굳이 리턴을 쓸필요없음*/
	return;
}

void InputSystem::KeyUp(unsigned int input)
{
	/*추가 : 배열의 크기보다 크면 오버플로나기 때문에 예외처리 해줘야한다.*/
	if (input >= 256) return;
	// If a key is released then clear that state in the key array.
	keys[input] = false;
	/*어차피 빠져나가기 때문에 굳이 리턴을 쓸필요없음*/
	return;
}

bool InputSystem::IsKeyDown(unsigned int key)
{
	/*추가 : 배열의 크기보다 크면 오버플로나기 때문에 예외처리 해줘야한다.*/
	if (key >= 256) return false;
	// Return what state the key is in (pressed/not pressed).
	return keys[key];
}