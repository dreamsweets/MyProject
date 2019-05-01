#pragma once
#include <string>

namespace MyComponent{

class IComponentBase {
protected:
	std::string name;
public:
	class GameObject* gameObject;
	virtual void Start() = 0;
	virtual void Update() = 0;
};

class Component : public IComponentBase {

};

class TransformComponent : public Component
{
};

class CameraComponent : public Component 
{

};
class MeshRendererComponent : public Component
{

};

}
