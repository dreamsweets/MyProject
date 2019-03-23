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


};

