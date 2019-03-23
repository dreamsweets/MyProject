#pragma once
//���� �� �����ϴ� ��� ������Ʈ�� ���ӿ�����Ʈ��� �����Ѵ�. ����, ī�޶�, �׿� ĳ����, ��� ���..
//���ӿ�����Ʈ�� ������Ʈ ������ �ô´�.
//����Ʈ ���ϱ� vector�� �ƴ϶� unordered map���� ������Ʈ�� �����ϴ���, unordered map�� ���ؼ� ���θ� �ؾ��ҵ�

#include "Precompiled.h"

using namespace std;

//�ֹ� �����̱� ������ ����� �̷��� ����.
class ComponentBase;

class GameObject
{
private:
	//������Ʈ�� �̸�
	string m_name;
	unordered_map<string, ComponentBase*> m_components;

public:
	GameObject();
	~GameObject();

	void Update();
};

