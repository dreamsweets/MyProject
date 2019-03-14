#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <string>
#include "Utility.h"

using namespace std;

class GameObject;
class Component
{
	friend class GameObject;
protected:
	GameObject* target;
public:
	Component();
	Component(GameObject* target);
	virtual ~Component();
	virtual void Start() {}
	virtual void Update() {}
};

class Transform : public Component {
public:
	Transform();
	Transform(GameObject* target);
	Transform(Vector3& position);
	Transform(GameObject* target, Vector3& position);
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

class MeshRenderer : public Component {
public:
	string mesh;
	MeshRenderer(const string& mesh);
	void Draw();
	virtual void Update();
};

class MoveManager : public Component {
	public:
		MoveManager();
		virtual void Update();

};
#endif // !_COMPONENT_H_


