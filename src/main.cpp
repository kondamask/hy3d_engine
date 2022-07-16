#include "platform/platform.h"

int main()
{
	platform_state platformState = {};
	if (PlatformInitialize(&platformState, "HY3D ENGINE", 640, 480))
	{
		while (PlatformProcessMessages(&platformState))
		{
			continue;
		}
	}
	return 0;
}