#pragma once

#include <Windows.h>
#include <assert.h>

//STL
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
using namespace std;



#define SAFE_RELEASE(p) { if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p) { if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p){ delete [] (p); (p) = NULL; } }