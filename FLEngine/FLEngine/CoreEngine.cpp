//싱글톤 패턴 구현이유 : 	return CoreEngine::getInstance()->MessageHandler(hwnd, umessage, wparam, lparam); 이짓하려고 만듦.
#include "CoreEngine.h"
#include "stdafx.h"
#include "WindowSystem.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"

CoreEngine* CoreEngine::coreEngine = nullptr;

CoreEngine& CoreEngine::getInstance()
{
	if (!coreEngine) coreEngine = new CoreEngine();
	return *coreEngine;
}

//Initialize. 시스템 객체 생성.
CoreEngine::CoreEngine() :
	window(nullptr),
	input(nullptr),
	graphics(nullptr)
{
	ApplicationHandle = this;
	int screenWidth = 0, screenHeight = 0;
	window = new WindowSystem;
	window->InitializeWindows(screenWidth, screenHeight);
}

bool CoreEngine::Frame()
{
	//사용자가 esc키를 눌렀고 프로그램을 종료하기를 원하는지 확인
	if (input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
}



//Shutdown()
CoreEngine::~CoreEngine()
{
	if (window) { 
		delete window;
		window = nullptr;
	}
}

void CoreEngine::Run()
{
	MSG msg;
	bool done, result;

	//메세지 구조체 초기화.
	ZeroMemory(&msg, sizeof(MSG));

	//윈도우나 사용자로부터 종료 메시지가 있을 때까지 돈다.
	done = false;
	while (!done)
	{
		//윈도우 메시지를 처리한다.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//윈도우가 어플리케이션 종료를 보내면 루프를 종료한다.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//종료 메시지 외에는 frame 처리를 한다.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

LRESULT CALLBACK CoreEngine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg)
	{
		//키보드상의 키가 눌렸는지를 확인한다.
	case WM_KEYDOWN:
	{
		//만약 키가 눌렸다면 input객체에게 key를 전달한다. 그래서 상태를 기록한다.
		input->KeyDown((unsigned int)wparam);
		return 0;
	}

	//키보드상의 키가 떼어졌는지를 확인한다.
	case WM_KEYUP:
	{
		//만약 키가 떼어졌다면 input객체에게 전달한다. 그래서 상태를 지운다.
		input->KeyUp((unsigned int)wparam);
		return 0;
	}

	//그 외 다른 모든 메시지는 default 메시지 처리기로 전달한다.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

