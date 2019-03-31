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

		//   �޸� �� ������ ����
		desc.handle = CreateWindowEx
		(
			WS_EX_APPWINDOW
			, desc.appName.c_str()  //   � ������ Ŭ���� ����ϴ���
			, desc.appName.c_str()  //   Ÿ��Ʋ �ٿ� ��� �̸�
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

		//   ȭ�� ����
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

		//   �����츦 ������ ȭ�鿡 ���
		ShowWindow(desc.handle, SW_SHOWNORMAL);
		//   ������ �ڵ� ��Ŀ�� �ֻ����� ����
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

	//Ű������� Ű�� ���ȴ����� Ȯ���Ѵ�.
	case WM_KEYDOWN:
	{
		//���� Ű�� ���ȴٸ� input��ü���� key�� �����Ѵ�. �׷��� ���¸� ����Ѵ�.
		//input->KeyDown((unsigned int)wparam);
		return 0;
	}

	//Ű������� Ű�� ������������ Ȯ���Ѵ�.
	case WM_KEYUP:
	{
		//���� Ű�� �������ٸ� input��ü���� �����Ѵ�. �׷��� ���¸� �����.
		//input->KeyUp((unsigned int)wparam);
		return 0;
	}

	case WM_CLOSE: case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	//�� �� �ٸ� ��� �޽����� default �޽��� ó����� �����Ѵ�.
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

	//�޼��� ����ü �ʱ�ȭ.
	ZeroMemory(&msg, sizeof(MSG));

	//�����쳪 ����ڷκ��� ���� �޽����� ���� ������ ����.
	done = false;
	while (!done)
	{
		//������ �޽����� ó���Ѵ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//�����찡 ���ø����̼� ���Ḧ ������ ������ �����Ѵ�.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//���� �޽��� �ܿ��� frame ó���� �Ѵ�.
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