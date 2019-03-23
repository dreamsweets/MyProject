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
	//windows api를 초기화한다.
	InitializeWindows(screenWidth, screenHeight);
	//input 객체 생성.
	m_Input = new InputClass;
	if (!m_Input) { return false; }
	m_Input->Initialize();
	//graphic 객체 생성.
	m_Graphics = new GraphicsClass;
	if (!m_Graphics) { return false; }
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result) { return false; }
	return true;
}


void SystemClass::Shutdown() { 
	// graphics 객체 해제.
	if(m_Graphics)
	{ m_Graphics->Shutdown();
	delete m_Graphics; m_Graphics = 0;
	}
	// input 객체 해제.
	if(m_Input)
	{ delete m_Input;
	m_Input = 0; 
	} 
	// 윈도우 shutdown.
	ShutdownWindows();
	return; 
}

void SystemClass::Run() {
	MSG msg;
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

	//사용자가 esc키를 눌렀고 프로그램을 종료하기를 원하는지 확인
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//graphic 객체의 Frame()함수 호출.
	result = m_Graphics->Frame();
	if (!result) { return false; }
	
	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg)
	{
		//키보드상의 키가 눌렸는지를 확인한다.
		case WM_KEYDOWN:
		{
			//만약 키가 눌렸다면 input객체에게 key를 전달한다. 그래서 상태를 기록한다.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

	//키보드상의 키가 떼어졌는지를 확인한다.
		case WM_KEYUP:
		{
			//만약 키가 떼어졌다면 input객체에게 전달한다. 그래서 상태를 지운다.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		//그 외 다른 모든 메시지는 default 메시지 처리기로 전달한다.
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

	//외부 포인터로 현재 객체의 주소를 담습니다.
	ApplicationHandle = this;

	//현재 어플리케이션의 인스턴스를 얻습니다.
	m_hinstance = GetModuleHandle(NULL);

	//어플리케이션 이름 설정
	m_applicationName = L"MyGameEngine";

	//윈도우 클래스를 기본으로 설정.
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

	//윈도우 클래스 등록.
	RegisterClassEx(&wc);

	//사용자 환경에 맞춘 화면크기 결정.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//전체 화면 여부에 따른 화면 관련 설정.
	if (FULL_SCREEN)
	{
		//만약 전체화면 모드이면 화면을 최대사이즈로 하고 32비트 컬러로 한다.
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

	//마우스커서 표시여부
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