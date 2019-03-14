#include "Component.h"
#include "Renderer.h"
#include "GameObject.h"
#include "InputManager.h"


Component::Component() : target(nullptr)
{
}

Component::Component(GameObject * target) : target(target)
{
}

Component::~Component()
{
}

MeshRenderer::MeshRenderer(const string& mesh):
	mesh(mesh)
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

Transform::Transform() : 
	position({0,0,0}),
	rotation({0,0,0}),
	scale({100,100,100})
{
}

Transform::Transform(GameObject * target) : Component(target)
{
}

Transform::Transform(Vector3& position) :
	position(position),
	rotation({ 0,0,0 }),
	scale({ 100,100,100 })
{
}

Transform::Transform(GameObject * target, Vector3 & position)
	: Component(target),
	position(position)
{
}

MoveManager::MoveManager()
{
}

void MoveManager::Update()
{
	if (InputManager::GetKeyDown(KeyCode::LeftArrow))
		--target->transform.position.x;
	if (InputManager::GetKeyDown(KeyCode::RightArrow))
		++target->transform.position.x;
}
