#pragma once
#include "GraphicsSystem.h"
class GraphicsSystem;
class CoreSystem {
public:
	CoreSystem();
	~CoreSystem();
	//void Initialize();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
	void Run();
	bool Frame();
private:
	static LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
private:
	GraphicsSystem* graphics;
};

