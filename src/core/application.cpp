#include "application.h"
#include "engine/engine_loader.cpp"

namespace HY3D
{
	bool ApplicationInitialize(application_config* appInfo)
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
		f64 frameStart = PlatformGetTime();
		f64 frameEnd = frameStart;
		f64 frameDt = frameEnd - frameStart;

		while (PlatformProcessMessages(&Application::state.platformState))
		{
			EngineReloadCode(&Application::state.engine, &Application::state.engineLibrary);

			if (!Application::state.isSuspended)
			{
				Application::state.engine.Update(frameDt);
				Application::state.engine.Render(frameDt);

				frameEnd = PlatformGetTime();
				frameDt = frameEnd - frameStart;
				frameStart = frameEnd;
#if LIMIT_FRAMES
				f64 remaningDt = 1.0 / TARGET_FPS - frameDt;
				if (remaningDt > 0)
				{
					PlatformSleep((u64)(remaningDt * 1000.0));
				}
#endif
			}
		}

		Application::state.engine.Terminate();
		PlatformTerminate(&Application::state.platformState);

		return true;
	}
}