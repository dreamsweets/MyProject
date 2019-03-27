//�̱��� ���� �������� : 	return CoreEngine::getInstance()->MessageHandler(hwnd, umessage, wparam, lparam); �����Ϸ��� ����.
#include "CoreEngine.h"
#include "Window.h"
#include "Graphics.h"
#include "Input.h"

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
	window = new Window;
	input = new Input;
	graphics = new Graphics;
}

bool CoreEngine::Frame()
{
	//����ڰ� escŰ�� ������ ���α׷��� �����ϱ⸦ ���ϴ��� Ȯ��
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

WNDCLASSEX CoreEngine::GetWNDCLASS()
{
	return wndClass;
}

void CoreEngine::SetWNDCLASS(WNDCLASSEX & wnd)
{
	wndClass = wnd;
}

