#include "application.h"
#include "engine/engine_loader.cpp"
#include "renderer/renderer_loader.cpp"

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

		EngineLoadCode(&Application::state.engine);
		RendererLoadCode(&Application::state.renderer, RENDERER_API_VULKAN);

		Application::state.engine.api.EngineInitialize();
		Application::state.renderer.api.RendererInitialize(&Application::state.platformState);

		return true;
	}

	bool ApplicationRun()
	{
		f64 frameStart = PlatformGetTime();
		f64 frameEnd = frameStart;
		f64 frameDt = frameEnd - frameStart;

		while (PlatformProcessMessages(&Application::state.platformState))
		{
			if (PlatformUpdatedDynamicLibrary(&Application::state.engine.library))
				EngineLoadCode(&Application::state.engine);
				
			if (PlatformUpdatedDynamicLibrary(&Application::state.renderer.library))
				RendererLoadCode(&Application::state.renderer, RENDERER_API_VULKAN);

			if (!Application::state.isSuspended)
			{
				Application::state.engine.api.EngineUpdate(frameDt);
				Application::state.engine.api.EngineRender(frameDt);

				render_packet packet;
				packet.dt = frameDt;
				Application::state.renderer.api.RendererDrawFrame(&packet);

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

		Application::state.renderer.api.RendererTerminate();
		Application::state.engine.api.EngineTerminate();
		PlatformTerminate(&Application::state.platformState);

		return true;
	}
}