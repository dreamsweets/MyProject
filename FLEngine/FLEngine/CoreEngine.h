//system ����, ���� ���� �Լ�, ������ �޼��� ó�� �� �ü���� ��ȣ�ۿ�� ������ �ٽ� ������ �����ϴ� ����� ����Ѵ�. �ý��� ����.
//WndProc�Լ����� MessageHandler�Լ��� ȣ���Ϸ��� �����ϴ�.. �̱��� ������ �ѹ� �Ẽ��? ���ϸ� ���߿� �����ϸ� ����~~
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

