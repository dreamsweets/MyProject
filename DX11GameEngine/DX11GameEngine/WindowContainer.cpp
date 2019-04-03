#include "WindowContainer.h"

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	OutputDebugStringA("Window PROC FROM WINDOW CONTAINER\n");
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
