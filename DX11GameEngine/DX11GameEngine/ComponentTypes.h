#pragma once
#include <string>
using namespace std;

class Component {
public:
	class GameObject* gameObject;
	virtual void Start() = 0;
	virtual void Update() = 0;
};

class CameraComponent : public Component {

};