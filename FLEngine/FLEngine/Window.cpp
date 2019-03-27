#include "Window.h"


//â ������ - D3Ddesc �޾ƿͼ� ������ �ٽ� D3Ddesc �ʱ�ȭ
Window::Window()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	WNDCLASSEX wndClass;
	wndClass = CoreEngine::getInstance().GetWNDCLASS();

	WORD wHr = RegisterClassEx(&wndClass);
	//assert(wHr != 0);
	if (desc.bFullScreen == true)
	{
		DEVMODE devMode = { 0 };
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmPelsWidth = (DWORD)desc.Width;
		devMode.dmPelsHeight = (DWORD)desc.Height;
		devMode.dmBitsPerPel = 32;
		devMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
	}

	desc.Handle = CreateWindowEx
	(
		WS_EX_APPWINDOW
		, desc.AppName.c_str()  //   � ������ Ŭ���� ����ϴ���
		, desc.AppName.c_str()  //   Ÿ��Ʋ �ٿ� ��� �̸�
		, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, NULL
		, (HMENU)NULL
		, desc.Instance
		, NULL
	);
	//assert(desc.Handle != NULL);
	D3D::SetDesc(desc);
	{
		RECT rect = { 0, 0, (LONG)desc.Width, (LONG)desc.Height };

		UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (UINT)desc.Width) / 2;
		UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (UINT)desc.Height) / 2;

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
		MoveWindow
		(
			desc.Handle
			, centerX, centerY
			, rect.right - rect.left, rect.bottom - rect.top
			, TRUE
		);
	}

	ShowWindow(desc.Handle, SW_SHOWNORMAL);
	//   ������ �ڵ� ��Ŀ�� �ֻ����� ����
	SetForegroundWindow(desc.Handle);
	SetFocus(desc.Handle);

	ShowCursor(true);
}


Window::~Window()
{
}
