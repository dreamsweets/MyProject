#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	SystemClass* System;
	bool result;

	//�ý��� ��ü ����
	System = new SystemClass;
	if (!System) {
		return 0;
	}

	//�ý��� �ʱ�ȭ�� Run() ȣ��
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	//�ý��� ��ü ������ �Ҵ� ����
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}