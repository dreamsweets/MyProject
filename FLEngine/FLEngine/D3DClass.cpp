#include "D3DClass.h"

D3DClass::D3DClass(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
	float screenDepth, float screenNear)
	:
	swapChain(0),
	device(0),
	deviceContext(0),
	renderTargetView(0),
	depthStencilBuffer(0),
	depthStencilState(0),
	depthStencilView(0),
	rasterState(0)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	size_t stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;

	//vsync ���� ����.(���� ����ȭ)
	vsync_enabled = vsync;

	//DirectX �׷��Ƚ� �������̽� ���丮 ����.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result)) MessageBox(hwnd, L"Direct3D CreateDXGIFactory ����", L"Error", MB_OK);
	//���� ī��� ������ adapter ������ ���� ���丮 ���. 0���̴ϱ� ù��° �׷���ī�� �������̽������� adapter.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) MessageBox(hwnd, L"Direct3D EnumAdapters ����", L"Error", MB_OK);
	//���(�����)�� ���� ù��° adapter ����.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) MessageBox(hwnd, L"Direct3D EnumOutputs ����", L"Error", MB_OK);
	//DXGI_FORMAT_R8G8B8A8_UNORM ����� ��� ���÷��� ���˿� �´� ����� ������ ����.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result)) MessageBox(hwnd, L"Direct3D GetDisplayModeList ����", L"Error", MB_OK);
	// �����/���� ī�� �� �� ������ ��� ��� ��� ������ ������ ����Ʈ ����.
	displayModeList = new DXGI_MODE_DESC[numModes];
	
	// ���÷��� ��忡 ���� ����Ʈ ������ ä���
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) MessageBox(hwnd, L"Direct3D GetDisplayModeList ä��� ����", L"Error", MB_OK);
	// ��� ���÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��� ��带 ã�´�.
	// ���� ã���� �ش� ������� �����(���ΰ�ħ ����)�� ����, �и� �����Ѵ�.
	for (i = 0; i<numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// �����(����ī��)�� description�� �����´�.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) MessageBox(hwnd, L"Direct3D adapter->GetDesc ����", L"Error", MB_OK);
	// ���� �׷���ī���� �޸� �뷮�� mb������ �ٲ㼭 �����Ѵ�.
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// �׷��� ī�� �̸��� char�� ���ڿ� �迭�� �ٲٰ� �����Ѵ�.
	error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if (!error) MessageBox(hwnd, L"�׷��� ī�� �̸� ���� ����", L"Error", MB_OK);
	// ���÷��� ��� ����Ʈ �Ҵ� ����
	delete[] displayModeList;
	displayModeList = 0;

	// ��� adapter �Ҵ� ����
	adapterOutput->Release();
	adapterOutput = 0;

	// adapter �Ҵ� ����
	adapter->Release();
	adapter = 0;

	// factory ��ü �Ҵ� ����
	factory->Release();
	factory = 0;

	// ���� ü�� description �ʱ�ȭ
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// �� ���� ����� ����ϵ��� ����
	swapChainDesc.BufferCount = 1;

	// ������� �ʺ�� ���� ����.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// ����ۿ��� DXGI_FORMAT_R8G8B8A8_UNORM (32bit �÷� ǥ��) ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// ����� ����� ����(���ΰ�ħ ����)
	if (vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// ������� �뵵 ����
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �������� �������� �ڵ� ����
	swapChainDesc.OutputWindow = hwnd;

	// ��Ƽ ���ø� �ɼ� ��
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// â������� ��üȭ������ ��� ����
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// ��ĵ������ ���İ� ��ĵ���̴� �ɼ��� �������� �������� ����
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ����� ���� ������� ���� ������
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰��ɼ� �÷��� ������ ����
	swapChainDesc.Flags = 0;

	// ���� ������ DirectX 11�� �����Ѵ�.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// ���� ü��, Direct3D ����̽�, Direct3D ����̽� ���ؽ�Ʈ ����
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);

	// ������� ������ ��������
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);


	//������� �����ͷ� ����Ÿ�� �並 ����
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);


	// ����� �����͸� ���̻� ������� �����Ƿ� �Ҵ� ����
	backBufferPtr->Release();
	backBufferPtr = 0;

	// ���� ������ description �ʱ�ȭ
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// ���� ������ description �ۼ�
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//description�� ����� ���� ������ �ؽ��ĸ� ����
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);

	//���ٽ� ������ description �ʱ�ȭ
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// ���ٽ� ������ description �ۼ�
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// �ȼ��� front-facing ������ �� ���ٽ� ���۰� �ؾߵǴ� �ൿ ����
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �ȼ��� back-facing ������ �� ���ٽ� ���۰� �ؾߵǴ� �ൿ ����
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ����-���ٽ� ���� ����
	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

	// ���� -���ٽ� ���� ����
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	// ���� - ���ٽ� ���� description �ʱ�ȭ
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// ���� - ���ٽ� ���� description �ۼ�
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// ���� - ���ٽ� ���� ����
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);

	// ����Ÿ�� ��� ���� -���ٽ� ���۸� ���� ��� ���������ο� ���ε��Ѵ�.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// � ������ ��� �׸� ������ �����ϴ� �����Ͷ����� ���� description �ۼ�
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

	// description�� �������� �����Ͷ����� ���� ����
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);

	// �����Ͷ����� ���� ����
	deviceContext->RSSetState(rasterState);

	// ������ ����Ʈ ����
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// ����Ʈ ����
	deviceContext->RSSetViewports(1, &viewport);

	// �������� ��Ʈ���� ����(���� ���)
	fieldOfView = (float)XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// ������� ����
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// ���� ����� ���� ��ķ� �ʱ�ȭ
	worldMatrix = XMMatrixIdentity();

	//���� ���� ��� ����(���ұ׷��� ���)
	orthographicMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
}

D3DClass::~D3DClass()
{
	if (swapChain) { swapChain->SetFullscreenState(false, NULL); }
	if (rasterState) { rasterState->Release(); rasterState = 0; }
	if (depthStencilView) { depthStencilView->Release(); depthStencilView = 0; }
	if (depthStencilState) { depthStencilState->Release(); depthStencilState = 0; }
	if (depthStencilBuffer) { depthStencilBuffer->Release(); depthStencilBuffer = 0; }
	if (renderTargetView) { renderTargetView->Release(); renderTargetView = 0; }
	if (deviceContext) { deviceContext->Release(); deviceContext = 0; }
	if (device) { device->Release(); device = 0; }
	if (swapChain) { swapChain->Release(); swapChain = 0; }

}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//����۸� ��ǲ �÷��� ����
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	//���� - ���ٽ� ���� ����.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene()
{
	//���� ����ȭ �ɼ�
	if (vsync_enabled) swapChain->Present(1, 0);
	else
		swapChain->Present(0, 0);
}

ID3D11Device * D3DClass::GetDevice()
{
	return device;
}

ID3D11DeviceContext * D3DClass::GetDeviceContext()
{
	return deviceContext;
}

void D3DClass::GetProjectionMatrix(XMMATRIX & projectionMatrix)
{
	projectionMatrix = this->projectionMatrix;
}

void D3DClass::GetWorldMatrix(XMMATRIX & worldMatrix)
{
	worldMatrix = this->worldMatrix;
}

void D3DClass::GetOrthographicMatrix(XMMATRIX & orthographicMatrix)
{
	orthographicMatrix = this->orthographicMatrix;
}

void D3DClass::GetVideoCardInfo(char * cardName, int & memory)
{
	strcpy_s(cardName, 128, videoCardDescription);
	memory = videoCardMemory;
}

