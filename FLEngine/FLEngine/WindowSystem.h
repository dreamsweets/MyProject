//info
//윈도우 창 생성을 담당한다. 그 이상하면 선 넘는거다. -> CoreEngine에 넣었더니 자꾸 에러가 나는걸.. WndProc함수가 윈도우 생성시 필요하니까 여기에 넣고, 딱 윈도우 창 꺼지는 것만 알 수 있도록 하면 되지~

#pragma once
#include "Precompiled.h"
#include "System.h"
#include "CoreEngine.h"

//윈도우 창 설정하는 전역변수들
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class WindowSystem :
	public System
{
public:
	WindowSystem();
	~WindowSystem();
	bool InitializeWindows(int&, int&);
	const HWND& GetHWND();
private:
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;
};

static WindowSystem * ApplicationHandle = nullptr;

