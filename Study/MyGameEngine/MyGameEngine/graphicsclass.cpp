#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
	:
	m_D3D(0),
	m_Camera(0),
	m_TextureShader(0),
	m_Model(0)
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//Direct3D ��ü ����
	m_D3D = new D3DClass;
	if (!m_D3D) { return false; }

	//Direct3D ��ü �ʱ�ȭ
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//ī�޶� ������Ʈ ����
	m_Camera = new CameraClass;
	if (!m_Camera) return false;
	
	//ī�޶� �ʱ� ��ġ ����
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	//�� ������Ʈ ����
	m_Model = new ModelClass;
	if (!m_Model) return false;

	//�� ������Ʈ�� �ʱ�ȭ
	result = m_Model->Initialize(m_D3D->GetDevice(), L"../MyGameEngine/data/test.png");
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the model object", L"Error", MB_OK);
		return false;
	}

	//�÷� ���̴� ������Ʈ ����
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader) return false;

	//�÷� ���̴� ������Ʈ �ʱ�ȭ
	m_TextureShader->Initialize(m_D3D->GetDevice(),hwnd);
	if (!result) {
		MessageBox(hwnd, L"Could not initialize the color shader object", L"Error", MB_OK);
		return false;
	}
	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	

	

	
	return;
}


bool GraphicsClass::Frame()
{
	bool result;

	//�׷��Ƚ� ����� �׸���.
	result = Render();
	if (!result) {
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	//����� �����ϱ� ���� ���۸� ����.
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_Camera->Render();

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_Model->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result) return false;

	//�׷��� ����� ȭ�鿡 ǥ���Ѵ�.
	m_D3D->EndScene();

	return true;
}
