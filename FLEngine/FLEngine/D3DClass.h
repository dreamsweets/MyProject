#pragma once

#include "Precompiled.h"
using namespace DirectX;
class D3DClass
{
public:
	D3DClass(int, int, bool, HWND, bool, float, float);
	~D3DClass();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthographicMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);
private:
	bool vsync_enabled;
	int videoCardMemory;
	char videoCardDescription[128];
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	//viewMatrix는 카메라 클래스에 들어감~
	XMMATRIX projectionMatrix;
	XMMATRIX worldMatrix;
	XMMATRIX orthographicMatrix;
};

