//�ڵ� ���Ĵٵ�
//��������� ���� m_ �� ���δ�.
//�Լ��� �빮�ڷ� ����.
//�ٹٲ��� �ؿ�ó�� {}�� Ȯ���ϰ� �����ǵ��� �Ѵ�.
//���� ������� ����
//������, �Ҹ��� �Լ��� Ȱ���� ������, �ƴϸ� Initialize�� Shutdown�Լ��� ����� ������...-> �ϴ� �Լ��� �ʹ� �������ϱ� �Ӹ�������. �׳� ������ �Ҹ��� ����.
#include "stdafx.h"
#include "CoreEngine.h"
#include "D3D.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	D3DDesc desc;
	desc.AppName = L"FL Engine";
	desc.Instance = hInstance;
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.Handle = NULL;
	desc.Width = 1280;
	desc.Height = 720;
	D3D::SetDesc(desc);

	CoreEngine::getInstance().Run();
	return 0;
}