#include "application.h"
#include "engine/engine_loader.cpp"
#include "renderer/renderer_loader.cpp"
#include "core/input.h"

namespace HY3D
{
	namespace Application
	{	
		bool ApplicationInitialize(application_config* appInfo)
		{
			if (state.isInitialized)
			{
				LOG_ERROR("Application '%s' is already initialized", appInfo->name);
				return false;
			}

			if (!PlatformInitialize(&state.platformState, appInfo->name, appInfo->width, appInfo->height))
			{
				return false;
			}

			EngineLoadCode(&state.engine);
			RendererLoadCode(&state.renderer, appInfo->renderer);

			state.engine.api.EngineInitialize();
			state.renderer.api.RendererInitialize(&state.platformState);
			
			if (!PlatformGetFileWriteTime("bin\\shaders\\triangle.vert.spv", &state.renderer.shadersWriteTime))
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

			bool changedRenderer = false;
			while (PlatformProcessMessages(&state.platformState))
			{
				if (!changedRenderer)
				{
					if (InputIsKeyPressed(input_button::KEYBOARD_BUTTON_1) &&
						!state.renderer.graphics_api == RENDERER_GRAPHICS_API_OPENGL)
					{
						changedRenderer = true;
						state.renderer.api.RendererTerminate(&state.renderer.apiContext);
						PlatformSetWindowTitle(&state.platformState, "HY3D Engine - OpenGL");
						RendererLoadCode(&state.renderer, RENDERER_GRAPHICS_API_OPENGL);
						state.renderer.api.RendererInitialize(&state.platformState);
					}
					else if (InputIsKeyPressed(input_button::KEYBOARD_BUTTON_2) &&
							!state.renderer.graphics_api == RENDERER_GRAPHICS_API_VULKAN)
					{
						changedRenderer = true;
						state.renderer.api.RendererTerminate(&state.renderer.apiContext);
						PlatformSetWindowTitle(&state.platformState, "HY3D Engine - Vulkan");
						RendererLoadCode(&state.renderer, RENDERER_GRAPHICS_API_VULKAN);
						state.renderer.api.RendererInitialize(&state.platformState);
					}
				}
				else if (changedRenderer && 
						(!InputIsKeyPressed(input_button::KEYBOARD_BUTTON_1) || 
						!InputIsKeyPressed(input_button::KEYBOARD_BUTTON_2)))
				{
					changedRenderer = false;
				}

	#if _DEBUG
				if (PlatformUpdatedDynamicLibrary(&state.engine.library))
					EngineLoadCode(&state.engine);
					
				if (PlatformUpdatedDynamicLibrary(&state.renderer.library))
					RendererLoadCode(&state.renderer, state.renderer.graphics_api);
	#endif

				if (!state.isSuspended)
				{
					state.engine.api.EngineUpdate(frameDt);
					state.engine.api.EngineRender(frameDt);

	#if _DEBUG
					if (PlatformWasFileUpdated("bin\\shaders\\triangle.vert.spv", &state.renderer.shadersWriteTime))
						state.renderer.api.RendererReloadShaders();
	#endif
					
					render_packet packet;
					packet.dt = frameDt;
					state.renderer.api.RendererDrawFrame(&packet);

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

			state.renderer.api.RendererTerminate(&state.renderer.apiContext);
			state.engine.api.EngineTerminate();
			PlatformTerminate(&state.platformState);

			return true;
		}
	} // namespace Application
}