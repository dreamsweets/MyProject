#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <string>
using namespace std;

class Component
{
protected:
	Component();
	virtual ~Component();
	virtual void start();
	virtual void update();
};

class Transform : public Component {
public:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};

class MeshRenderer : public Component {
public:
	string mesh;
	void Draw();
};
#endif // !_COMPONENT_H_


