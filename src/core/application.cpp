#include "application.h"
#include "engine/engine_loader.cpp"

namespace HY3D
{
	bool ApplicationInitialize(application_config *appInfo)
	{
		if (Application::state.isInitialized)
		{
			LOG_ERROR("Application '%s' is already initialized", appInfo->name);
			return false;
		}

		if (!PlatformInitialize(&Application::state.platformState, appInfo->name, appInfo->width, appInfo->height))
		{
			return false;
		}

		EngineLoadCode(&Application::state.engine, &Application::state.engineLibrary);

		Application::state.engine.Initialize();

		return true;
	}

	bool ApplicationRun()
	{
		while (PlatformProcessMessages(&Application::state.platformState))
		{
			EngineReloadCode(&Application::state.engine, &Application::state.engineLibrary);

			if (!Application::state.isSuspended)
			{
				Application::state.engine.Update();
				Application::state.engine.Render();
			}
		}

		Application::state.engine.Terminate();
		PlatformTerminate(&Application::state.platformState);

		return true;
	}
}