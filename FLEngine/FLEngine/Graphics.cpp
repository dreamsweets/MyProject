#include "Graphics.h"


Graphics::Graphics()
	:	D3D (D3D::Get())
{
}


Graphics::~Graphics()
{
	SAFE_DELETE(D3D);
}

bool Graphics::Frame()
{
	return Render() ? true : false;
}

bool Graphics::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	Color color = { 0.5f, 0.5f, 0.5f, 1.0f };
	D3D->BeginScene(color);

	D3D->Present();
	return true;
}
