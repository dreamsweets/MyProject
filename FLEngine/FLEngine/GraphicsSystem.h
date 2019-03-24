//렌더링 작업만을 담당하는 클래스. 화면에 뭔가 나와야 할 때 출력 해주는 놈
#pragma once
#include "System.h"
#include "Precompiled.h"
#include "D3DClass.h"
#include "WindowSystem.h"

class GraphicsSystem :
	public System
{
public:
	GraphicsSystem(int screenWidth, int screenHeight, HWND hwnd);
	~GraphicsSystem();

	bool Frame();
private:
	//d3dclass를 이용해 화면 초기화 및 그리기 명령을 캡슐화함.
	bool Render();

private:
	D3DClass* D3D;

};

