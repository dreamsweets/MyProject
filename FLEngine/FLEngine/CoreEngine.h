//system 제어, 게임 루프 함수, 윈도우 메세지 처리 등 운영체제와 상호작용과 게임의 핵심 루프를 제어하는 기능을 담당한다.
//WndProc함수에서 MessageHandler함수를 호출하려니 복잡하다.. 싱글톤 패턴을 한번 써볼까? 망하면 나중에 수정하면 되지~~
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
	//특별한 놈들이라 대문자로 표현해줌. -> 이상한데? 이렇게 해도 돼? 일단 규칙 맞춰서 소문자로 해.
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
