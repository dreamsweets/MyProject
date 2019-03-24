//������ �۾����� ����ϴ� Ŭ����. ȭ�鿡 ���� ���;� �� �� ��� ���ִ� ��
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
	//d3dclass�� �̿��� ȭ�� �ʱ�ȭ �� �׸��� ����� ĸ��ȭ��.
	bool Render();

private:
	D3DClass* D3D;

};

