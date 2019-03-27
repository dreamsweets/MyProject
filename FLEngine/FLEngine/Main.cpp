//�ڵ� ���Ĵٵ�
//��������� ���� m_ �� ���δ�.
//�Լ��� �빮�ڷ� ����.
//�ٹٲ��� �ؿ�ó�� {}�� Ȯ���ϰ� �����ǵ��� �Ѵ�.
//���� ������� ����
//������, �Ҹ��� �Լ��� Ȱ���� ������, �ƴϸ� Initialize�� Shutdown�Լ��� ����� ������...-> �ϴ� �Լ��� �ʹ� �������ϱ� �Ӹ�������. �׳� ������ �Ҹ��� ����.
#include "stdafx.h"
#include "CoreEngine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	D3DDesc desc;
	desc.AppName = L"Frontline Engine";
	desc.Instance = hInstance;
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.Handle = NULL;
	desc.Width = 1280;
	desc.Height = 720;
	D3D::SetDesc(desc);

	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hInstance = desc.Instance;
	wndClass.lpfnWndProc = (WNDPROC)CoreEngine::getInstance().WinProc;
	wndClass.lpszClassName = desc.AppName.c_str();
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.hIconSm = wndClass.hIcon;
	CoreEngine::SetWNDCLASS(wndClass);

	CoreEngine::getInstance().Run();
	return 0;
}