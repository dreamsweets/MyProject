//싱글톤 패턴 구현이유 : 	return CoreEngine::getInstance()->MessageHandler(hwnd, umessage, wparam, lparam); 이짓하려고 만듦.
#include "CoreEngine.h"
#include "Window.h"
#include "Graphics.h"
#include "Input.h"
#include "D3D.h"

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
	D3DDesc desc;
	D3D::GetDesc(&desc);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hInstance = desc.Instance;
	wndClass.lpfnWndProc = (WNDPROC)((CoreEngine::getInstance()).WinProc);
	wndClass.lpszClassName = (LPCWSTR)(desc.AppName).c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.hIconSm = wndClass.hIcon;

	window = new Window;
	input = new Input;
	graphics = new Graphics;
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
}

void CoreEngine::Run()
{
	MSG msg = { 0 };
	bool done, result;

	D3DDesc desc;
	D3D::SetDesc(desc);
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
			if (!result) done = true;
		}
	}
	return;
}

void CoreEngine::Shutdown()
{
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	SAFE_DELETE(window);
}

LRESULT CALLBACK CoreEngine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg)
	{
	case WM_SIZE :
		{
		//ImGui::Invalidate();
			float width = (float)LOWORD(lparam);
			float height = (float)HIWORD(lparam);

			if (D3D::Get() != NULL)
				D3D::Get()->ResizeScreen(width, height);
		}

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

	case WM_CLOSE: case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	//그 외 다른 모든 메시지는 default 메시지 처리기로 전달한다.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

LRESULT CoreEngine::WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
	{
		//ImGui::Invalidate();
		{
			float width = (float)LOWORD(lParam);
			float height = (float)HIWORD(lParam);

			if (D3D::Get() != NULL)
				D3D::Get()->ResizeScreen(width, height);
		}
		//ImGui::Validate();
	}
	case WM_CLOSE:case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return DefWindowProc(handle, message, wParam, lParam);
		}
	}

}

void CoreEngine::GetWndClass(WNDCLASSEX * wnd)
{
	*wnd = wndClass;
}


