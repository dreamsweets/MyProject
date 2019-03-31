//코딩 스탠다드
//멤버변수에 굳이 m_ 안 붙인다.
//함수는 대문자로 쓰자.
//줄바꿈은 밑에처럼 {}로 확실하게 구별되도록 한다.
//아직 고민중인 문제
//생성자, 소멸자 함수를 활용할 것인지, 아니면 Initialize와 Shutdown함수를 사용할 것인지...-> 일단 함수가 너무 많아지니까 머리아프다. 그냥 생성자 소멸자 쓰자.
#include "stdafx.h"
#include "CoreEngine.h"
#include "D3D.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	D3DDesc desc;
	desc.AppName = L"FL Engine";
	desc.Instance = hInstance;
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.Handle = NULL;
	desc.Width = 1280;
	desc.Height = 720;
	D3D::SetDesc(desc);

	CoreEngine::getInstance().Run();
	return 0;
}