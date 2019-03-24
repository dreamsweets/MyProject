#pragma once

//�⺻, �ܺ� ���̺귯�� ��
#include <vector>
#include <Windows.h>
#include <string>
#include <unordered_map>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h> // DirectXMath�� ���Չ�ٰ� �ؼ� 10�� �Ⱦ��� �� �̰ɷ� �ٲ�. ���� ����.https://docs.microsoft.com/ko-kr/windows/desktop/dxmath/pg-xnamath-migration-d3dx
/*
������ ���� ����ü �̸��� �ٲ����.

D3DXMath Type	DirectXMath Equivalent
D3DXFLOAT16	HALF
D3DXMATRIX	XMFLOAT4X4
D3DXMATRIXA16	XMMATRIX or XMFLOAT4X4A
D3DXQUATERNION
D3DXPLANE
D3DXCOLOR
XMVECTOR is used rather than having unique types, so you will likely need to use an XMFLOAT4
[!Note]
[D3DXQUATERNION::operator *](https://msdn.microsoft.com/en-us/library/Bb205422(v=VS.85).aspx) calls the D3DXQuaternionMultiply function, which multiplies two quaternions. But, unless you explicitly use the XMQuaternionMultiply function, you get an incorrect answer when you use XMVECTOR::operator * on a quaternion.

D3DXVECTOR2	XMFLOAT2
D3DXVECTOR2_16F	XMHALF2
D3DXVECTOR3	XMFLOAT3
D3DXVECTOR4	XMFLOAT4(or if you can guarantee the data is 16 - byte aligned, XMVECTOR or XMFLOAT4A)
D3DXVECTOR4_16F	XMHALF4
*/
//����