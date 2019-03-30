//info
//������ â ������ ����Ѵ�. �� �̻��ϸ� �� �Ѵ°Ŵ�. -> CoreEngine�� �־����� �ڲ� ������ ���°�.. WndProc�Լ��� ������ ������ �ʿ��ϴϱ� ���⿡ �ְ�, �� ������ â ������ �͸� �� �� �ֵ��� �ϸ� ����~

#pragma once
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
	void InitializeWindows(int&, int&);

	//HWND�� ��ȯ�ϴ� �Լ�
	HWND& getHWND();
private:
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;
};



