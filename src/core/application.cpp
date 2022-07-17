#include "application.h"

namespace HY3D
{
	static_func bool ApplicationInitialize(application_config *appInfo)
	{
		if (appState.isInitialized)
		{
			LOG_ERROR("Application '%s' is already initialized", appInfo->name)
			return false;
		}

		if (!PlatformInitialize(&appState.platformState, appInfo->name, appInfo->width, appInfo->height))
		{
			return false;
		}

		return true;
	}

	static_func bool ApplicationRun()
	{
		while (PlatformProcessMessages(&appState.platformState))
		{
			if (!appState.isSuspended)
			{
				// TODO: Update
				// TODO: Render
			}
		}

		PlatformTerminate(&appState.platformState);

		return true;
	}
}