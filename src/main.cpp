#include "core/application.h"

using namespace HY3D;

static_func int Start()
{
	application_config config = {
		.width    = 640,
		.height   = 480,
		.name     = "HY3D Engine",
		.renderer = RENDERER_GRAPHICS_API_VULKAN};
	if (Application::ApplicationInitialize(&config))
	{
		Application::ApplicationRun();
	}
	return 0;
}


//----------------------------------------------------------------------------------

#if _DEBUG
int main()
{
	return Start();
}
#else

//----------------------------------------------------------------------------------

#if PLATFORM_WINDOWS
#include "platform/win32/hy3d_windows.h"
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return Start();
}
#endif

//----------------------------------------------------------------------------------

#endif