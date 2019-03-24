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

	//vsync 설정 저장.(수직 동기화)
	vsync_enabled = vsync;

	//DirectX 그래픽스 인터페이스 팩토리 생성.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(result)) MessageBox(hwnd, L"Direct3D CreateDXGIFactory 실패", L"Error", MB_OK);
	//비디오 카드로 적용할 adapter 생성을 위해 팩토리 사용. 0번이니까 첫번째 그래픽카드 인터페이스에대한 adapter.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) MessageBox(hwnd, L"Direct3D EnumAdapters 실패", L"Error", MB_OK);
	//출력(모니터)에 대한 첫번째 adapter 나열.
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) MessageBox(hwnd, L"Direct3D EnumOutputs 실패", L"Error", MB_OK);
	//DXGI_FORMAT_R8G8B8A8_UNORM 모니터 출력 디스플레이 포맷에 맞는 모드의 개수를 구함.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result)) MessageBox(hwnd, L"Direct3D GetDisplayModeList 실패", L"Error", MB_OK);
	// 모니터/비디오 카드 둘 다 가능한 모든 출력 모드 조합을 저장할 리스트 생성.
	displayModeList = new DXGI_MODE_DESC[numModes];
	
	// 디스플레이 모드에 대한 리스트 구조를 채우기
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) MessageBox(hwnd, L"Direct3D GetDisplayModeList 채우기 실패", L"Error", MB_OK);
	// 모든 디스플레이 모드에 대해 화면 너비/높이에 맞는 디스플레이 모드를 찾는다.
	// 만약 찾으면 해당 모니터의 재생률(새로고침 비율)의 분자, 분모를 저장한다.
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

	// 어댑터(비디오카드)의 description을 가져온다.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) MessageBox(hwnd, L"Direct3D adapter->GetDesc 실패", L"Error", MB_OK);
	// 현재 그래픽카드의 메모리 용량을 mb단위로 바꿔서 저장한다.
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 그래픽 카드 이름을 char형 문자열 배열로 바꾸고 저장한다.
	error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
	if (!error) MessageBox(hwnd, L"그래픽 카드 이름 저장 실패", L"Error", MB_OK);
	// 디스플레이 모드 리스트 할당 해제
	delete[] displayModeList;
	displayModeList = 0;

	// 출력 adapter 할당 해제
	adapterOutput->Release();
	adapterOutput = 0;

	// adapter 할당 해제
	adapter->Release();
	adapter = 0;

	// factory 객체 할당 해제
	factory->Release();
	factory = 0;

	// 스왑 체인 description 초기화
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// 한 개의 백버퍼 사용하도록 설정
	swapChainDesc.BufferCount = 1;

	// 백버퍼의 너비와 높이 설정.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// 백버퍼에도 DXGI_FORMAT_R8G8B8A8_UNORM (32bit 컬러 표준) 지정
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 백버퍼 재생율 지정(새로고침 비율)
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

	// 백버퍼의 용도 지정
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 렌더링할 윈도우의 핸들 지정
	swapChainDesc.OutputWindow = hwnd;

	// 멀티 샘플링 옵션 끔
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// 창모드인지 전체화면인지 모드 설정
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// 스캔라인의 정렬과 스캔라이닝 옵션을 지정되지 않음으로 설정
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 출력한 이후 백버퍼의 내용 버리기
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가옵션 플래그 사용안함 설정
	swapChainDesc.Flags = 0;

	// 피쳐 레벨을 DirectX 11로 설정한다.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// 스왑 체인, Direct3D 디바이스, Direct3D 디바이스 컨텍스트 생성
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);

	// 백버퍼의 포인터 가져오기
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);


	//백버퍼의 포인터로 렌더타겟 뷰를 생성
	result = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);


	// 백버퍼 포인터를 더이상 사용하지 않으므로 할당 해제
	backBufferPtr->Release();
	backBufferPtr = 0;

	// 깊이 버퍼의 description 초기화
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// 깊이 버퍼의 description 작성
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

	//description을 사용해 깊이 버퍼의 텍스쳐를 생성
	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);

	//스텐실 상태의 description 초기화
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// 스텐실 상태의 description 작성
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// 픽셀이 front-facing 상태일 때 스텐실 버퍼가 해야되는 행동 설정
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 픽셀이 back-facing 상태일 때 스텐실 버퍼가 해야되는 행동 설정
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이-스텐실 상태 생성
	result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

	// 깊이 -스텐실 상태 설정
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	// 깊이 - 스텐실 뷰의 description 초기화
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// 깊이 - 스텐실 뷰의 description 작성
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// 깊이 - 스텐실 뷰의 생성
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);

	// 렌더타겟 뷰와 깊이 -스텐실 버퍼를 각각 출력 파이프라인에 바인딩한다.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// 어떤 도형을 어떻게 그릴 것인지 결정하는 래스터라이저 상태 description 작성
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

	// description을 바탕으로 래스터라이저 상태 생성
	result = device->CreateRasterizerState(&rasterDesc, &rasterState);

	// 래스터라이저 상태 설정
	deviceContext->RSSetState(rasterState);

	// 렌더링 뷰포트 설정
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// 뷰포트 생성
	deviceContext->RSSetViewports(1, &viewport);

	// 프로젝션 매트릭스 설정(투영 행렬)
	fieldOfView = (float)XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// 투영행렬 생성
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// 월드 행렬을 단위 행렬로 초기화
	worldMatrix = XMMatrixIdentity();

	//직교 투영 행렬 설정(오소그래픽 행렬)
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

	//백버퍼를 인풋 컬러로 리셋
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	//깊이 - 스텐실 버퍼 지움.
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3DClass::EndScene()
{
	//수직 동기화 옵션
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

