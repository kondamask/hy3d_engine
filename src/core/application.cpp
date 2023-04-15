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

		Application::state.engine.Initialize();
		Application::state.renderer.Initialize(&Application::state.platformState);
		
		if (!PlatformGetFileWriteTime("bin\\shaders\\triangle.vert.spv", &Application::state.renderer.shadersWriteTime))
		{
			LOG_ERROR("Could not find compiled shaders.");
			return false;
		}

		return true;
	}

	bool ApplicationRun()
	{
		f64 frameStart = PlatformGetTime();
		f64 frameEnd = frameStart;
		f64 frameDt = frameEnd - frameStart;

		while (PlatformProcessMessages(&Application::state.platformState))
		{
#if _DEBUG
			if (PlatformUpdatedDynamicLibrary(&Application::state.engine.library))
				EngineLoadCode(&Application::state.engine);
				
			if (PlatformUpdatedDynamicLibrary(&Application::state.renderer.library))
				RendererLoadCode(&Application::state.renderer, RENDERER_API_VULKAN);
#endif

			if (!Application::state.isSuspended)
			{
				Application::state.engine.Update(frameDt);
				// Application::state.engine.Render(frameDt);

#if _DEBUG
				if (PlatformWasFileUpdated("bin\\shaders\\triangle.vert.spv", &Application::state.renderer.shadersWriteTime))
					Application::state.renderer.ReloadShaders();
#endif
				
				render_packet packet;
				packet.dt = frameDt;
				Application::state.renderer.DrawFrame(&packet);

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

		Application::state.renderer.Terminate();
		Application::state.engine.Terminate();
		PlatformTerminate(&Application::state.platformState);

		return true;
	}
}