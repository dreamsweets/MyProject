#include "RenderWindow.h"

bool RenderWindow::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringConverter::StringToWide(this->window_title);
	this->window_class = window_class;
	this->window_class_wide = StringConverter::StringToWide(this->window_class);

	this->RegisterWindowClass();

	this->handle = CreateWindowEx(0,
		this->window_class_wide.c_str(),
		this->window_class_wide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0,
		0,
		this->width,
		this->height,
		NULL,
		NULL,
		this->hInstance,
		nullptr);

	if (this->handle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "CreateWindowEx Failed for window: " + this->window_title);
		return false;
	}

	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool RenderWindow::ProcessMessages()
{
	// Handle the windows messages.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // Initialize the message structure.

	if (PeekMessage(&msg, //Where to store message (if one exists) See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
		this->handle, //Handle to window we are checking messages for
		0,    //Minimum Filter Msg Value - We are not filtering for specific messages, but the min/max could be used to filter only mouse messages for example.
		0,    //Maximum Filter Msg Value
		PM_REMOVE))//Remove message after capturing it via PeekMessage. For more argument options, see: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644943(v=vs.85).aspx
	{
		TranslateMessage(&msg); //Translate message from virtual key messages into character messages so we can dispatch the message. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644955(v=vs.85).aspx
		DispatchMessage(&msg); //Dispatch message to our Window Proc for this window. See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms644934(v=vs.85).aspx
	}

	// Check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL; //Message processing loop takes care of destroying this window
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}
void RenderWindow::RegisterWindowClass() 
{
	WNDCLASSEX wc = { };
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = this->window_class_wide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}
RenderWindow::~RenderWindow()
{
	if (this->handle != NULL)
	{
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(handle);
	}
}
