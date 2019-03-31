#include "../stdafx.h"
#include "CoreSystem.h"
#include "D3D.h"
#include "GraphicsSystem.h"

CoreSystem::CoreSystem():
	graphics(new GraphicsSystem)
{
	D3DDesc desc;
	D3D::GetD3DDesc(&desc);

		WNDCLASSEX wndClass;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.hInstance = desc.instance;
		wndClass.lpfnWndProc = (WNDPROC)WinProc;
		wndClass.lpszClassName = desc.appName.c_str();
		wndClass.lpszMenuName = NULL;
		wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.hIconSm = wndClass.hIcon;
		WORD wHr = RegisterClassEx(&wndClass);
		assert(wHr != 0);

		if (desc.bFullScreen == true)
		{
			DEVMODE devMode = { 0 };
			devMode.dmSize = sizeof(DEVMODE);
			devMode.dmPelsWidth = (DWORD)desc.width;
			devMode.dmPelsHeight = (DWORD)desc.height;
			devMode.dmBitsPerPel = 32;
			devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
		}

		//   메모리 상에 윈도우 생성
		desc.handle = CreateWindowEx
		(
			WS_EX_APPWINDOW
			, desc.appName.c_str()  //   어떤 윈도우 클래스 사용하는지
			, desc.appName.c_str()  //   타이틀 바에 띄울 이름
			, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
			, CW_USEDEFAULT
			, CW_USEDEFAULT
			, CW_USEDEFAULT
			, CW_USEDEFAULT
			, NULL
			, (HMENU)NULL
			, desc.instance
			, NULL
		);
		assert(desc.handle != NULL);
		D3D::SetD3DDesc(desc);

		//   화면 맞춤
		{
			RECT rect = { 0, 0, (LONG)desc.width, (LONG)desc.height };

			UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.width) / 2;
			UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.height) / 2;

			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			MoveWindow
			(
				desc.handle
				, centerX, centerY
				, rect.right - rect.left, rect.bottom - rect.top
				, TRUE
			);
		}

		//   윈도우를 실제로 화면에 띄움
		ShowWindow(desc.handle, SW_SHOWNORMAL);
		//   윈도우 핸들 포커스 최상위로 설정
		SetForegroundWindow(desc.handle);
		SetFocus(desc.handle);

		ShowCursor(true);
}

CoreSystem::~CoreSystem()
{
	SAFE_DELETE(graphics);
}

LRESULT CALLBACK CoreSystem::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg)
	{
	case WM_SIZE:
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
		//input->KeyDown((unsigned int)wparam);
		return 0;
	}

	//키보드상의 키가 떼어졌는지를 확인한다.
	case WM_KEYUP:
	{
		//만약 키가 떼어졌다면 input객체에게 전달한다. 그래서 상태를 지운다.
		//input->KeyUp((unsigned int)wparam);
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

void CoreSystem::Run()
{
	MSG msg = { 0 };
	bool done, result;

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

bool CoreSystem::Frame()
{
	graphics->Frame();
	return true;
}

LRESULT CoreSystem::WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
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
			return DefWindowProc(handle, message, wParam, lParam);
	}
}