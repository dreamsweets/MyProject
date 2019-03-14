#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <string>
#include "Utility.h"

using namespace std;

class GameObject;
class Component
{
public:
	const GameObject* target;
	Component(const GameObject* target);
	virtual ~Component();
	virtual void Start() {}
	virtual void Update() {}
};

class Transform : public Component {
public:
	Transform(const GameObject* target);
	Transform(const GameObject* target, Vector3 position);
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

class MeshRenderer : public Component {
public:
	string mesh;
	MeshRenderer(const GameObject* target, string mesh);
	void Draw();
	virtual void Update();
};

class MoveManager : public Component {
	public:
		MoveManager(const GameObject* target);
		virtual void Update();

};
#endif // !_COMPONENT_H_


