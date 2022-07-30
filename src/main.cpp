#include "core/application.h"

using namespace HY3D;

static_func int Start()
{
	application_config config = {640, 480, "HY3D Engine"};
	if (ApplicationInitialize(&config))
	{
		if (!ApplicationRun())
		{
			LOG_ERROR("Application stopped running unexpectedly");
		}
	}
	return 0;
}

#if _DEBUG
// This entry point will crete a command prompt where we print messages
// The other entry points just creates the window.
int main()
{
	return Start();
}
#else

#if PLATFORM_WINDOWS
#include "platform/win32/hy3d_windows.h"
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	return Start();
}
#endif

#endif