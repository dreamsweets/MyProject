//�ڵ� ���Ĵٵ�
//��������� ���� m_ �� ���δ�.
//�Լ��� �빮�ڷ� ����.
//�ٹٲ��� �ؿ�ó�� {}�� Ȯ���ϰ� �����ǵ��� �Ѵ�.
//���� ������� ����
//������, �Ҹ��� �Լ��� Ȱ���� ������, �ƴϸ� Initialize�� Shutdown�Լ��� ����� ������...-> �ϴ� �Լ��� �ʹ� �������ϱ� �Ӹ�������. �׳� ������ �Ҹ��� ����.

#include "Precompiled.h"
#include "CoreEngine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	CoreEngine::getInstance().Run();

	return 0;
}