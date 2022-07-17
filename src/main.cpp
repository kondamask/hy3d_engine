#include "platform/platform.h"

int main()
{
	LOG_FATAL("test");
	LOG_ERROR("test");
	LOG_DEBUG("test");
	LOG_INFO("test");
	LOG_WARN("test");
	LOG_TRACE("test");
	platform_state platformState = {};
	if (PlatformInitialize(&platformState, "HY3D ENGINE", 640, 480))
	{
		while (PlatformProcessMessages(&platformState))
		{
			continue;
		}
	}
	PlatformTerminate(&platformState);
	return 0;
}