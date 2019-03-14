#include "GameObject.h"



GameObject::GameObject() : name("GameObject"), transform(*(new Transform(this)))
{

}

GameObject::GameObject(string name) :name(name), transform(*(new Transform(this)))
{
}

GameObject::GameObject(string name, Vector3 pos): name(name), transform(*(new Transform(this, pos)))
{
}

GameObject::GameObject(Vector3 pos) :GameObject()
{
	transform.position = pos;
}


GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (auto component : components) {
		component->Update();
	}
}

void GameObject::AddComponent(Component * component)
{
	components.push_back(component);
}

