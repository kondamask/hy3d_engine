#include "application.h"
#include "engine/engine_stubs.h"

namespace HY3D
{
#if _DEBUG
#define ENGINE_DLL "bin\\Debug\\engine.dll"
#else
#define ENGINE_DLL "bin\\Release\\engine.dll"
#endif

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
		PlatformBindToLogger(&Logger::API);

		dynamic_library engineLibrary = {};
		if (PlatformLoadDynamicLibrary(ENGINE_DLL, &engineLibrary))
		{
			Application::state.engine.Initialize = (pfnEngineInitialize)PlatformGetDynamicLibraryFunction(&engineLibrary, "EngineInitialize");
			Application::state.engine.Update = (pfnEngineUpdate)PlatformGetDynamicLibraryFunction(&engineLibrary, "EngineUpdate");
			Application::state.engine.Render = (pfnEngineRender)PlatformGetDynamicLibraryFunction(&engineLibrary, "EngineRender");
			Application::state.engine.Terminate = (pfnEngineTerminate)PlatformGetDynamicLibraryFunction(&engineLibrary, "EngineTerminate");
		}
		EngineValidateAPI(&Application::state.engine);

		PlatformBindToEngine(&Application::state.engine.platformAPI);

		Application::state.engine.Initialize(&Application::state.engine);

		return true;
	}

	bool ApplicationRun()
	{
		while (PlatformProcessMessages(&Application::state.platformState))
		{
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