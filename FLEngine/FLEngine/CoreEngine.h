//system 제어, 게임 루프 함수, 윈도우 메세지 처리 등 운영체제와 상호작용과 게임의 핵심 루프를 제어하는 기능을 담당한다. 시스템 모음.
//WndProc함수에서 MessageHandler함수를 호출하려니 복잡하다.. 싱글톤 패턴을 한번 써볼까? 망하면 나중에 수정하면 되지~~
#pragma once
#include "stdafx.h"

class Window;
class Input;
class Graphics;

class CoreEngine
{
	static CoreEngine* coreEngine;
public:
	static CoreEngine& getInstance();
	

	void Run();
	void Shutdown();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	static WNDCLASSEX GetWNDCLASS();
	static void SetWNDCLASS(WNDCLASSEX&);
private:
	CoreEngine();
	~CoreEngine();
	bool Frame();
private:
	WNDCLASSEX wndClass;
	Window* window;
	Input* input;
	Graphics* graphics;


};

