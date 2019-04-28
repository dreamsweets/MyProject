#pragma once
#include <vector>
#include <map>
#include <string>
using namespace std;
class Component;

class GameObject
{
	string name;
	GameObject* parent;
	vector<GameObject*> children;
	//map<string, Component> components;
public:
	GameObject(string name = "Game Object");
	~GameObject();
};

