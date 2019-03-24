//�̱��� ���� �������� : 	return CoreEngine::getInstance()->MessageHandler(hwnd, umessage, wparam, lparam); �����Ϸ��� ����.
#include "CoreEngine.h"

#include "WindowSystem.h"
#include "GraphicsSystem.h"
#include "InputSystem.h"

CoreEngine* CoreEngine::coreEngine = nullptr;

CoreEngine& CoreEngine::getInstance()
{
	if (!coreEngine) coreEngine = new CoreEngine();
	return *coreEngine;
}

//Initialize. �ý��� ��ü ����.
CoreEngine::CoreEngine() :
	window(nullptr),
	input(nullptr),
	graphics(nullptr)
{
	int screenWidth = 0, screenHeight = 0;
	window = new WindowSystem();
	if(window->InitializeWindows(screenWidth, screenHeight))
		graphics = new GraphicsSystem(screenWidth, screenHeight, window->GetHWND());
	input = new InputSystem();
	
}


//Shutdown()
CoreEngine::~CoreEngine()
{
	if (window) {
		delete window;
		window = nullptr;
	}
	/* ������ �Ҵ��ص״����� �����Ѵ�.*/
	if (input)
	{
		delete input;
		input = nullptr;
	}
	if (graphics)
	{
		delete graphics;
		graphics = nullptr;
	}
}


bool CoreEngine::Frame()
{
	//����ڰ� escŰ�� ������ ���α׷��� �����ϱ⸦ ���ϴ��� Ȯ��
	if (input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	/* �߰� */
	return true;
}



void CoreEngine::Run()
{
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

LRESULT CALLBACK CoreEngine::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg)
	{
		//Ű������� Ű�� ���ȴ����� Ȯ���Ѵ�.
		case WM_KEYDOWN:
		{
			//���� Ű�� ���ȴٸ� input��ü���� key�� �����Ѵ�. �׷��� ���¸� ����Ѵ�.
			input->KeyDown((unsigned int)wparam);
			return 0;
		}

		//Ű������� Ű�� ������������ Ȯ���Ѵ�.
		case WM_KEYUP:
		{
			//���� Ű�� �������ٸ� input��ü���� �����Ѵ�. �׷��� ���¸� �����.
			input->KeyUp((unsigned int)wparam);
			return 0;
		}

		//�� �� �ٸ� ��� �޽����� default �޽��� ó����� �����Ѵ�.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

//LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
//{
//	switch (umessage)
//	{
//		// Check if the window is being destroyed.
//	case WM_DESTROY:
//	{
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	// Check if the window is being closed.
//	case WM_CLOSE:
//	{
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	// All other messages pass to the message handler in the system class.
//	default:
//	{
//		/*����*/
//		return CoreEngine::getInstance()->MessageHandler(hwnd, umessage, wparam, lparam);
//	}
//	}
//}

