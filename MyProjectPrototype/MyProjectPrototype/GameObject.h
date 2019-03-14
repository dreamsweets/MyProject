#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <string>
#include <vector>
#include "Component.h"
using namespace std;

class GameObject
{
public:
	Transform& transform;
	string name;
	vector<Component*> components;
	GameObject();
	GameObject(string name);
	GameObject(string name, Vector3 pos);
	GameObject(Vector3 pos);
	virtual ~GameObject();
	void Update();
	void AddComponent(Component* component);
};

#endif // !_GAMEOBJECT_H_


