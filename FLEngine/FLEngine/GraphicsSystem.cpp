#include "GraphicsSystem.h"



GraphicsSystem::GraphicsSystem(int screenWidth, int screenHeight, HWND hwnd)
	:
	D3D(nullptr)
{
	D3D = new D3DClass(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!D3D) MessageBox(hwnd, L"Direct3D Initialize ½ÇÆĞ", L"Error", MB_OK); 
}


GraphicsSystem::~GraphicsSystem()
{
	if (D3D)
	{
		delete D3D;
		D3D = nullptr;
	}
}

bool GraphicsSystem::Frame()
{
	if (!Render()) return false;
	return true;
}

bool GraphicsSystem::Render()
{
	D3D->BeginScene(0.5f, 0.5, 0.5f, 1.0f);
	D3D->EndScene();
	return true;
}
