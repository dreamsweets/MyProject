#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <string>
#include <map>
#include "Component.h"
using namespace std;

class GameObject
{
public:
	string name;
	map<string, Component*> components;
	GameObject();
	GameObject(string name);
	virtual ~GameObject();
	void AddComponent(Component* component){}
};

#endif // !_GAMEOBJECT_H_


