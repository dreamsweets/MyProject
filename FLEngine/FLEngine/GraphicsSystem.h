//렌더링 작업만을 담당하는 클래스. 화면에 뭔가 나와야 할 때 출력 해주는 놈
#pragma once
#include "System.h"
#include "Precompiled.h"

class GraphicsSystem :
	public System
{
public:
	GraphicsSystem();
	~GraphicsSystem();
};

