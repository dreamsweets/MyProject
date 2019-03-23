//system 제어, 게임 루프 함수, 윈도우 메세지 처리 등 운영체제와 상호작용과 게임의 핵심 루프를 제어하는 기능을 담당한다.
#pragma once

#include "Precompiled.h"
#include "System.h"

class CoreEngine
{
private:
	System* system;
public:
	CoreEngine();
	~CoreEngine();
};

