//system ����, ���� ���� �Լ�, ������ �޼��� ó�� �� �ü���� ��ȣ�ۿ�� ������ �ٽ� ������ �����ϴ� ����� ����Ѵ�.
//WndProc�Լ����� MessageHandler�Լ��� ȣ���Ϸ��� �����ϴ�.. �̱��� ������ �ѹ� �Ẽ��? ���ϸ� ���߿� �����ϸ� ����~~
#pragma once
#include "Precompiled.h"

class WindowSystem;
class InputSystem;
class GraphicsSystem;

class CoreEngine
{
	static CoreEngine* coreEngine;
public:
	static CoreEngine& getInstance();

	~CoreEngine();

	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	CoreEngine();
	bool Frame();
private:
	//Ư���� ����̶� �빮�ڷ� ǥ������. -> �̻��ѵ�? �̷��� �ص� ��? �ϴ� ��Ģ ���缭 �ҹ��ڷ� ��.
	WindowSystem* window;
	InputSystem* input;
	GraphicsSystem* graphics;
	HWND hwnd;

};

//static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return CoreEngine::getInstance().MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}
