#include "Component.h"
#include "Renderer.h"
#include "GameObject.h"
#include "InputManager.h"


Component::Component(const GameObject* target) : target(target)
{
}

Component::~Component()
{
}

MeshRenderer::MeshRenderer(const GameObject * target, string mesh):
	Component(target), mesh(mesh)
{
}

void MeshRenderer::Draw()
{
	Renderer::getInstance().Draw((target->transform.position), mesh);
}

void MeshRenderer::Update()
{
	Draw();
}

Transform::Transform(const GameObject* target) : 
	Component(target),
	position({0,0,0}),
	rotation({0,0,0}),
	scale({100,100,100})
{
}

Transform::Transform(const GameObject * target, Vector3 position) :
	Component(target),
	position(position),
	rotation({ 0,0,0 }),
	scale({ 100,100,100 })
{
}

MoveManager::MoveManager(const GameObject * target) : Component(target)
{
}

void MoveManager::Update()
{
	if (InputManager::GetKeyDown(KeyCode::LeftArrow))
		--target->transform.position.x;
	if (InputManager::GetKeyDown(KeyCode::RightArrow))
		++target->transform.position.x;
}
