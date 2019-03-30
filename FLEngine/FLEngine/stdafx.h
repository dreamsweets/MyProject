#pragma once
#include <Windows.h>

//#include <dxgi1_2.h>
//#include <d3dcommon.h>
//#include <d3dcompiler.h>
//#include <d3d11shader.h>
//#include <d3d11.h>

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <list>
#include <functional>
using namespace std;

#include "Util.h"

#define SAFE_RELEASE(p) { if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p) { if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p){ delete [] (p); (p) = NULL; } }

//XMMATRIX -> Matrix