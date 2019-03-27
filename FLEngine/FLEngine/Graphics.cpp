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

	D3D->Clear();

	D3D->Present();
	return true;
}
