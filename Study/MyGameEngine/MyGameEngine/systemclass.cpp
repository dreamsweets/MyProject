#include "systemclass.h"

SystemClass::SystemClass() {
	m_Input = 0;
	m_Graphics = 0;
}

SystemClass::SystemClass(const SystemClass& other) {

}

SystemClass::~SystemClass()
{

}

bool SystemClass::Initialize()
{
	int screenWidth=0, screenHeight=0;
	bool result;
	//windows api�� �ʱ�ȭ�Ѵ�.
	InitializeWindows(screenWidth, screenHeight);
	//input ��ü ����.
	m_Input = new InputClass;
	if (!m_Input) { return false; }
	m_Input->Initialize();
	//graphic ��ü ����.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics) { return false; }
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result) { return false; }
	return true;
}


void SystemClass::Shutdown() { 
	// graphics ��ü ����.
	if(m_Graphics)
	{ m_Graphics->Shutdown();
	delete m_Graphics; m_Graphics = 0;
	}
	// input ��ü ����.
	if(m_Input)
	{ delete m_Input;
	m_Input = 0; 
	} 
	// ������ shutdown.
	ShutdownWindows();
	return; 
}

void SystemClass::Run() {
	MSG msg;
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
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

bool SystemClass::Frame() {
	bool result;

	//����ڰ� escŰ�� ������ ���α׷��� �����ϱ⸦ ���ϴ��� Ȯ��
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//graphic ��ü�� Frame()�Լ� ȣ��.
	result = m_Graphics->Frame();
	if (!result) { return false; }
	
	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg)
	{
		//Ű������� Ű�� ���ȴ����� Ȯ���Ѵ�.
		case WM_KEYDOWN:
		{
			//���� Ű�� ���ȴٸ� input��ü���� key�� �����Ѵ�. �׷��� ���¸� ����Ѵ�.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

	//Ű������� Ű�� ������������ Ȯ���Ѵ�.
		case WM_KEYUP:
		{
			//���� Ű�� �������ٸ� input��ü���� �����Ѵ�. �׷��� ���¸� �����.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		//�� �� �ٸ� ��� �޽����� default �޽��� ó����� �����Ѵ�.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight) {
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//�ܺ� �����ͷ� ���� ��ü�� �ּҸ� ����ϴ�.
	ApplicationHandle = this;

	//���� ���ø����̼��� �ν��Ͻ��� ����ϴ�.
	m_hinstance = GetModuleHandle(NULL);

	//���ø����̼� �̸� ����
	m_applicationName = L"MyGameEngine";

	//������ Ŭ������ �⺻���� ����.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//������ Ŭ���� ���.
	RegisterClassEx(&wc);

	//����� ȯ�濡 ���� ȭ��ũ�� ����.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//��ü ȭ�� ���ο� ���� ȭ�� ���� ����.
	if (FULL_SCREEN)
	{
		//���� ��üȭ�� ����̸� ȭ���� �ִ������� �ϰ� 32��Ʈ �÷��� �Ѵ�.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//���콺Ŀ�� ǥ�ÿ���
	ShowCursor(true);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}