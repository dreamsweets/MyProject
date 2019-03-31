#include "stdafx.h"
#include "System\CoreSystem.h"
#include "System\D3D.h"

int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE prvinstancem, LPSTR cmdLine, int cmdShow)
{
	D3DDesc desc;
	desc.appName = L"D3DGame";
	desc.instance = hinstance;
	desc.bFullScreen = false;
	desc.bVsync = false;
	desc.handle = NULL;
	desc.width = 1280;
	desc.height = 720;
	D3D::SetD3DDesc(desc);

	CoreSystem* coreSystem = new CoreSystem;
	coreSystem->Run();
}