#pragma once
#include <vector>
#include <map>
using namespace std;
class Component;

class GameObject
{
	GameObject* parent;
	vector<GameObject*> children;
	map<string, Component> components;
public:
	GameObject();
	~GameObject();
};

