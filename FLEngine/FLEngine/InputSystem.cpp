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
	/*�߰� : �迭�� ũ�⺸�� ũ�� �����÷γ��� ������ ����ó�� ������Ѵ�.*/
	if (input >= 256) return;
	keys[input] = true;
	/*������ ���������� ������ ���� ������ ���ʿ����*/
	return;
}

void InputSystem::KeyUp(unsigned int input)
{
	/*�߰� : �迭�� ũ�⺸�� ũ�� �����÷γ��� ������ ����ó�� ������Ѵ�.*/
	if (input >= 256) return;
	// If a key is released then clear that state in the key array.
	keys[input] = false;
	/*������ ���������� ������ ���� ������ ���ʿ����*/
	return;
}

bool InputSystem::IsKeyDown(unsigned int key)
{
	/*�߰� : �迭�� ũ�⺸�� ũ�� �����÷γ��� ������ ����ó�� ������Ѵ�.*/
	if (key >= 256) return false;
	// Return what state the key is in (pressed/not pressed).
	return keys[key];
}