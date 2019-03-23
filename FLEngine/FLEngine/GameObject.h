#pragma once
//���� �� �����ϴ� ��� ������Ʈ�� ���ӿ�����Ʈ��� �����Ѵ�. ����, ī�޶�, �׿� ĳ����, ��� ���..
//���ӿ�����Ʈ�� ������Ʈ ������ �ô´�.
//����Ʈ ���ϱ� vector�� �ƴ϶� unordered map���� ������Ʈ�� �����ϴ���, unordered map�� ���ؼ� ���θ� �ؾ��ҵ�

#include <string>
#include <unordered_map>
using namespace std;

//�ֹ� �����̱� ������ ����� �̷��� ����.
class ComponentBase;

class GameObject
{
protected:
	//������Ʈ�� �̸�
	string m_name;
	unordered_map<string, ComponentBase> m_components;

public:
	GameObject();
	~GameObject();

	void Update();
};

