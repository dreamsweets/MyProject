#pragma once
#include "D3D.h"

class GraphicsSystem
{
public:
	GraphicsSystem();
	~GraphicsSystem();
	void Frame();
private:
	void Render();
	D3D* d3d;
};

