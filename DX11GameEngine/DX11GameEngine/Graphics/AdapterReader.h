#pragma once
#include "../ErrorLogger.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <vector> 

class AdapterData
{
public:
	AdapterData(IDXGIAdapter * pAdapter);
	IDXGIAdapter * pAdapter = nullptr;
	DXGI_ADAPTER_DESC description;
};

//static vector�� ������ ���� : Ȥ�ó� Window�� 2�� �̻��� �Ǿ��� �� ���� �۾��� 2�� �ݺ��ϱ� ������ �ʱ� ������
class AdapterReader
{
public:
	static std::vector<AdapterData> GetAdapters();
private:
	static std::vector<AdapterData> adapters;
};
