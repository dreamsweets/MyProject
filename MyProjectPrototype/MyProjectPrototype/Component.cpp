#include "Component.h"
#include "Renderer.h"



Component::Component()
{
}


Component::~Component()
{
}

void MeshRenderer::Draw()
{
	Renderer::getInstance().Draw();
}
