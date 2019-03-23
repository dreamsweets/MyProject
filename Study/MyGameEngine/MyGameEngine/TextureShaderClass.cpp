#include "TextureShaderClass.h"



TextureShaderClass::TextureShaderClass() 
	: 
	m_vertexShader(nullptr),
	m_pixelShader(nullptr),
	m_layout(nullptr),
	m_matrixBuffer(nullptr),
	m_sampleState(nullptr)
{}

TextureShaderClass::TextureShaderClass(const TextureShaderClass &)
{
}


TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	result = InitializeShader(device, hwnd, L"../MyGameEngine/texture.vs", L"../MyGameEngine/texture.ps");
	if(!result) return false;

	return true;
}

void TextureShaderClass::Shutdown()
{
	ShutdownShader();
	return;
}

bool TextureShaderClass::Render(ID3D11DeviceContext *deviceContext, int indexCount, D3DXMATRIX worldMatrix,
	D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView * texture)
{
	bool result;

	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result) return false;

	RenderShader(deviceContext, indexCount);

	return true;
}

bool TextureShaderClass::InitializeShader(ID3D11Device *device, HWND hwnd, WCHAR * vsFilename, WCHAR * psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result)) {
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		else
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
	return false;
	}

	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TexturePixelShader",
		"ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);

	if (FAILED(result)) {
		if (errorMessage)
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		else
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		return false;
	}


	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		NULL, &m_vertexShader);
	if (FAILED(result)) return false;
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result)) return false;

	//vertex Input layout description 생성.
	//이 description 셋업시 Modelclass랑 shader쪽에서 만든 구조체랑 동일한 구조를 가져야한다.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(polygonLayout, numElements, 
		vertexShaderBuffer->GetBufferPointer(),vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result)) return false;
	
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result)) return false;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result)) return false;

	return true;
}

//bool TextureShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
//{
//	HRESULT result;
//	ID3D10Blob* errorMessage;
//	ID3D10Blob* vertexShaderBuffer;
//	ID3D10Blob* pixelShaderBuffer;
//	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
//	unsigned int numElements;
//	D3D11_BUFFER_DESC matrixBufferDesc;
//	D3D11_SAMPLER_DESC samplerDesc;
//
//
//	// Initialize the pointers this function will use to null.
//	errorMessage = 0;
//	vertexShaderBuffer = 0;
//	pixelShaderBuffer = 0;
//
//	// Compile the vertex shader code.
//	result = D3DX11CompileFromFile(vsFilename, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
//		&vertexShaderBuffer, &errorMessage, NULL);
//	if (FAILED(result))
//	{
//		// If the shader failed to compile it should have writen something to the error message.
//		if (errorMessage)
//		{
//			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
//		}
//		// If there was nothing in the error message then it simply could not find the shader file itself.
//		else
//		{
//			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
//		}
//
//		return false;
//	}
//
//	// Compile the pixel shader code.
//	result = D3DX11CompileFromFile(psFilename, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
//		&pixelShaderBuffer, &errorMessage, NULL);
//	if (FAILED(result))
//	{
//		// If the shader failed to compile it should have writen something to the error message.
//		if (errorMessage)
//		{
//			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
//		}
//		// If there was  nothing in the error message then it simply could not find the file itself.
//		else
//		{
//			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
//		}
//
//		return false;
//	}
//
//	// Create the vertex shader from the buffer.
//	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Create the pixel shader from the buffer.
//	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Create the vertex input layout description.
//	// This setup needs to match the VertexType stucture in the ModelClass and in the shader.
//	polygonLayout[0].SemanticName = "POSITION";
//	polygonLayout[0].SemanticIndex = 0;
//	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
//	polygonLayout[0].InputSlot = 0;
//	polygonLayout[0].AlignedByteOffset = 0;
//	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//	polygonLayout[0].InstanceDataStepRate = 0;
//
//	polygonLayout[1].SemanticName = "TEXCOORD";
//	polygonLayout[1].SemanticIndex = 0;
//	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
//	polygonLayout[1].InputSlot = 0;
//	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
//	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
//	polygonLayout[1].InstanceDataStepRate = 0;
//
//	// Get a count of the elements in the layout.
//	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
//
//	// Create the vertex input layout.
//	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
//		&m_layout);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
//	vertexShaderBuffer->Release();
//	vertexShaderBuffer = 0;
//
//	pixelShaderBuffer->Release();
//	pixelShaderBuffer = 0;
//
//	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
//	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
//	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	matrixBufferDesc.MiscFlags = 0;
//	matrixBufferDesc.StructureByteStride = 0;
//
//	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
//	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	// Create a texture sampler state description.
//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.MipLODBias = 0.0f;
//	samplerDesc.MaxAnisotropy = 1;
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
//	samplerDesc.BorderColor[0] = 0;
//	samplerDesc.BorderColor[1] = 0;
//	samplerDesc.BorderColor[2] = 0;
//	samplerDesc.BorderColor[3] = 0;
//	samplerDesc.MinLOD = 0;
//	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//
//	// Create the texture sampler state.
//	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
//	if (FAILED(result))
//	{
//		return false;
//	}
//
//	return true;
//}
void TextureShaderClass::ShutdownShader()
{
	if (m_sampleState) {
		m_sampleState->Release();
		m_sampleState = 0;
	}
	if (m_matrixBuffer) {
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}
	if (m_pixelShader) {
		m_pixelShader->Release();
		m_pixelShader = 0;
	}
	if (m_vertexShader) {
		m_vertexShader->Release();
	m_vertexShader = 0;
	}

	return;
}

void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob * errorMessage, HWND hwnd, WCHAR *shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	//에러 메세지 텍스트 버퍼의 포인터 받아오기
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();

	//에러 메시지를 쓸 파일 열기
	fout.open("shader-error.txt");

	//메세지 입력
	for (i = 0; i < bufferSize; i++)
		fout << compileErrors[i];
	fout.close();

	errorMessage->Release();
	errorMessage = 0;
	//유저한테 셰이더 파일 컴파일 에러났으니 텍스트파일확인하라고 팝업창 띄우기
	MessageBox(hwnd, L"셰이더 컴파일 에러입니다. shader-error.txt파일 확인 요망.", shaderFilename, MB_OK);

	return;
}

bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext * deviceContext, 
	D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, 
	ID3D11ShaderResourceView * texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	deviceContext->Unmap(m_matrixBuffer, 0);
	bufferNumber = 0;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

void TextureShaderClass::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	deviceContext->IASetInputLayout(m_layout);

	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	deviceContext->DrawIndexed(indexCount, 0, 0);
	return;
}



