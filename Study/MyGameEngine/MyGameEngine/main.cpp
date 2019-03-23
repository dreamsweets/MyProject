#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	SystemClass* System;
	bool result;

	//시스템 객체 생성
	System = new SystemClass;
	if (!System) {
		return 0;
	}

	//시스템 초기화후 Run() 호출
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	//시스템 객체 정리후 할당 해제
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}