#pragma once
//게임 상에 존재하는 모든 오브젝트를 게임오브젝트라고 정의한다. 조명, 카메라, 그외 캐릭터, 배경 등등..
//게임오브젝트는 컴포넌트 관리를 맡는다.
//사이트 보니까 vector가 아니라 unordered map으로 컴포넌트를 저장하던데, unordered map에 대해서 공부를 해야할듯

#include "Precompiled.h"

using namespace std;

//쌍방 참조이기 때문에 헤더에 이렇게 선언.
class ComponentBase;

class GameObject
{
private:
	//오브젝트의 이름
	string m_name;
	unordered_map<string, ComponentBase*> m_components;

public:
	GameObject();
	~GameObject();

	void Update();
};

