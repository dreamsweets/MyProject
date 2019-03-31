#include "../stdafx.h"
#include "GraphicsSystem.h"



GraphicsSystem::GraphicsSystem()
	:	d3d(D3D::Get())
{
}

GraphicsSystem::~GraphicsSystem()
{
	SAFE_DELETE(d3d);
}

void GraphicsSystem::Frame()
{
	Render();
}

void GraphicsSystem::Render()
{
	d3d->BeginScene(0.5f,0.5f,0.5f,1.0f);
	d3d->EndScene();
}
