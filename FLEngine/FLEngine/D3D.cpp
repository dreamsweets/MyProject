#include "D3D.h"

D3D* D3D::instance = nullptr;
D3DDesc D3D::d3dDesc;

D3D * D3D::Get()
{
	if(!instance) 	instance = new D3D();
	return instance;
}
void D3D::SetGpuInfo()
{
	HRESULT hr;

	IDXGIFactory* factory;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter;
	hr = factory->EnumAdapters(0, &adapter);
	assert(SUCCEEDED(hr));

	IDXGIOutput* adapterOutput;
	hr = adapter->EnumOutputs(0, &adapterOutput);
	assert(SUCCEEDED(hr));

	//   이 모드에 지원하는 GPU 전부 가져옴
	UINT modeCount;
	hr = adapterOutput->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM
		, DXGI_ENUM_MODES_INTERLACED
		, &modeCount
		, NULL
	);
	assert(SUCCEEDED(hr));

	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[modeCount];
	hr = adapterOutput->GetDisplayModeList
	(
		DXGI_FORMAT_R8G8B8A8_UNORM
		, DXGI_ENUM_MODES_INTERLACED
		, &modeCount
		, displayModeList
	);
	assert(SUCCEEDED(hr));

	for (UINT i = 0; i < modeCount; i++)
	{
		bool isCheck = true;
		isCheck &= displayModeList[i].Width == d3dDesc.Width;
		isCheck &= displayModeList[i].Height == d3dDesc.Height;

		if (isCheck == true)
		{
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	hr = adapter->GetDesc(&adapterDesc);
	assert(SUCCEEDED(hr));

	gpuMemorySize = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
	gpuDescription = adapterDesc.Description;

	SAFE_DELETE_ARRAY(displayModeList);


	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);
}
void D3D::CreateSwapChainAndDevice()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	{
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = 0;
		swapChainDesc.BufferDesc.Height = 0;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; 
	
	if (d3dDesc.bVsync == true)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = d3dDesc.Handle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !d3dDesc.bFullScreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	}

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//지원 되는 DX버전
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1, // windows 10에서만 사용가능.
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		NULL
		, D3D_DRIVER_TYPE_HARDWARE
		, NULL
		, creationFlags
		, featureLevels
		, 1
		, D3D11_SDK_VERSION
		, &swapChainDesc
		, &swapChain
		, &device
		, NULL
		, &deviceContext
	);
	assert(SUCCEEDED(hr));   //   그래픽 카드가 위 DX버전을 지원하지 않을 때
}
void D3D::CreateBackBuffer(float width, float height)
{
	HRESULT hr;

		//   백버퍼 생성 영역
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = (UINT)width;
		desc.Height = (UINT)height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		hr = D3D::GetDevice()->CreateTexture2D(&desc, NULL, &backBuffer);
		assert(SUCCEEDED(hr));
	}

	{
		ID3D11Texture2D* backbufferPointer;
		hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backbufferPointer);
		assert(SUCCEEDED(hr));

		hr = D3D::GetDevice()->CreateRenderTargetView(backbufferPointer, NULL, &renderTargetView);
		assert(SUCCEEDED(hr));

		SAFE_RELEASE(backbufferPointer);
	}
}
void D3D::DeleteBackBuffer()
{
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(renderTargetView);
	SAFE_RELEASE(backBuffer);
}
void D3D::CreateDepthStencilBuffer()
{
	HRESULT hr;
	{	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		//description
		{
		// Depth Buffer 생성 인터페이스
		depthBufferDesc.Width = d3dDesc.Width;
		depthBufferDesc.Height = d3dDesc.Height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
	}
		hr = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
		assert(SUCCEEDED(hr)); 
	}

	{	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		//description
		{
		// DepthStencilDescription 생성 인터페이스
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}
		hr = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
		assert(SUCCEEDED(hr));
	deviceContext->OMSetDepthStencilState(depthStencilState, 1); 
	}

	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		//description
		{
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;
		}
		hr = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
		assert(SUCCEEDED(hr));

		deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	}
}
void D3D::CreateRasterizer()
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = device->CreateRasterizerState(&rasterDesc, &rasterState);
	assert(SUCCEEDED(hr));

	// Now set the rasterizer state.
	deviceContext->RSSetState(rasterState);
}
void D3D::CreateViewport()
{
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)d3dDesc.Width;
	viewport.Height = (float)d3dDesc.Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	deviceContext->RSSetViewports(1, &viewport);
}
void D3D::SetupMatrix()
{
	float fieldOfView, screenAspect;

	fieldOfView = (float)XM_PI / 4.0f;
	screenAspect = (float)d3dDesc.Width / (float)d3dDesc.Height;

	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);

	worldMatrix = XMMatrixIdentity();

	orthoMatrix = XMMatrixOrthographicLH((float)d3dDesc.Width, (float)d3dDesc.Height, SCREEN_NEAR, SCREEN_DEPTH);

}
void D3D::GetVideoCardInfo(char * cardName, int & memory)
{
	strncpy(cardName, (char*)gpuDescription.c_str(), 128);
	memory = (int)gpuMemorySize;;
}
//void D3D::Clear(Color& color, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
//{
//
//
//	if (!rtv)
//		rtv = renderTargetView;
//
//	if (!dsv)
//		dsv = depthStencilView;
//
//
//}
bool D3D::BeginScene(Color &color)
{
	float _color[4];
	_color[0] = color.r;
	_color[1] = color.g;
	_color[2] = color.b;
	_color[3] = color.a;
	deviceContext->ClearRenderTargetView(renderTargetView, _color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	return true;
}
void D3D::Present()
{
	swapChain->Present(d3dDesc.bVsync == true ? 1 : 0, 0);
}
void D3D::ResizeScreen(float width, float height)
{
	if (width < 1 || height < 1)
		return;

	d3dDesc.Width = width;
	d3dDesc.Height = height;

	DeleteBackBuffer();
	{
		HRESULT hr = swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0);
		assert(SUCCEEDED(hr));
	}
	CreateBackBuffer(width, height);
}
D3D::D3D()
	: numerator(0), denominator(1)
	,swapChain(nullptr)
	,device(nullptr)
	,deviceContext(nullptr)
	,renderTargetView(nullptr)
	,depthStencilBuffer(nullptr)
	,depthStencilState(nullptr)
	,rasterState(nullptr)
{
	SetGpuInfo();
	CreateSwapChainAndDevice();
	CreateBackBuffer(d3dDesc.Width, d3dDesc.Height);
	CreateDepthStencilBuffer();
	CreateRasterizer();
	CreateViewport();
	SetupMatrix();
}


D3D::~D3D()
{
	DeleteBackBuffer();

	if (deviceContext != NULL)
		deviceContext->ClearState();

	if (swapChain != NULL)
		swapChain->SetFullscreenState(false, NULL);

	SAFE_RELEASE(rasterState);
	SAFE_RELEASE(depthStencilView);
	SAFE_RELEASE(depthStencilState);
	SAFE_RELEASE(depthStencilBuffer);
	SAFE_RELEASE(renderTargetView);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
	SAFE_RELEASE(swapChain);

	SAFE_DELETE(instance);
}
