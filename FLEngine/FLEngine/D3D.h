#pragma once
#include "stdafx.h"

#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
// D3DDesc에 있는 값을 가지고 D3D 초기화
struct D3DDesc
{
	wstring AppName;
	HINSTANCE Instance;
	HWND Handle;
	float Width;
	float Height;
	bool bVsync;
	bool bFullScreen;
};

class D3D
{
public:

	static D3D* Get();

	static void GetDesc(D3DDesc* desc)
	{
		*desc = d3dDesc;
	}

	static void SetDesc(D3DDesc& desc)
	{
		d3dDesc = desc;
	}

	ID3D11Device* GetDevice()
	{
		return device;
	}

	ID3D11DeviceContext* GetDC()
	{
		return deviceContext;
	}

	/*void SetRenderTarget(ID3D11RenderTargetView* rtv = NULL, ID3D11DepthStencilView* dsv = NULL);*/

	//void Clear(Color& color = { 0,0,0,0 }, ID3D11RenderTargetView * rtv = nullptr, ID3D11DepthStencilView * dsv = nullptr);
	bool BeginScene(Color& color);
	void Present();

	void ResizeScreen(float width, float height);

	void GetProjectionMatrix(XMMATRIX& matrix)
	{
		matrix = projectionMatrix;
	}
	void GetWorldMatrix(XMMATRIX& matrix)
	{
		matrix = worldMatrix;
	}
	void GetOrthoMatrix(XMMATRIX&matrix)
	{
		matrix = orthoMatrix;
	}
	void GetVideoCardInfo(char*, int&);
	~D3D();
private:
	D3D();

	void SetGpuInfo();
	void CreateSwapChainAndDevice();
	void CreateBackBuffer(float width, float height);
	void DeleteBackBuffer();
	void CreateDepthStencilBuffer();
	void CreateRasterizer();
	void CreateViewport();
	void SetupMatrix();

private:
	static D3D* instance;

	bool vsync_enabled;

	UINT numerator;
	UINT denominator;
	UINT gpuMemorySize;
	wstring gpuDescription;

	ID3D11Texture2D* backBuffer;

	static D3DDesc d3dDesc;
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterState;
	XMMATRIX projectionMatrix;
	XMMATRIX worldMatrix;
	XMMATRIX orthoMatrix;
};

