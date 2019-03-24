//info
//������ â ������ ����Ѵ�. �� �̻��ϸ� �� �Ѵ°Ŵ�. -> CoreEngine�� �־����� �ڲ� ������ ���°�.. WndProc�Լ��� ������ ������ �ʿ��ϴϱ� ���⿡ �ְ�, �� ������ â ������ �͸� �� �� �ֵ��� �ϸ� ����~

#pragma once
#include "Precompiled.h"
#include "System.h"
#include "CoreEngine.h"

//������ â �����ϴ� ����������
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

