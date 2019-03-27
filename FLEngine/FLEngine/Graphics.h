#pragma once
#include "stdafx.h"
#include "System.h"

class Graphics : public System
{
public:
	Graphics();
	~Graphics();

	bool Frame();
private:
	bool Render();

private:
	D3D* D3D;
};

