#include "application.h"

namespace HY3D
{
#if _DEBUG
#define ENGINE_DLL "bin\\Debug\\engine.dll"
#else
#define ENGINE_DLL "bin\\Release\\engine.dll"
#endif

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

		dynamic_library engineLibrary = {};
		if (PlatformLoadDynamicLibrary(ENGINE_DLL, &engineLibrary))
		{
			appState.engine.Initialize = (pfnEngineInitialize)PlatformGetDynamicLibraryFunction(&engineLibrary, "EngineInitialize");
			appState.engine.Update = (pfnEngineUpdate)PlatformGetDynamicLibraryFunction(&engineLibrary, "EngineUpdate");
			appState.engine.Render = (pfnEngineRender)PlatformGetDynamicLibraryFunction(&engineLibrary, "EngineRender");
			appState.engine.Terminate = (pfnEngineTerminate)PlatformGetDynamicLibraryFunction(&engineLibrary, "EngineTerminate");
		}

		else
		{
			appState.engine.Initialize = EngineInitializeSTUB;
			appState.engine.Update = EngineUpdateSTUB;
			appState.engine.Render = EngineRenderSTUB;
			appState.engine.Terminate = EngineTerminateSTUB;
		}
		appState.engine.Initialize();

		return true;
	}

	static_func bool ApplicationRun()
	{
		while (PlatformProcessMessages(&appState.platformState))
		{
			if (!appState.isSuspended)
			{
				appState.engine.Update();
				appState.engine.Render();
			}
		}

		appState.engine.Terminate();
		PlatformTerminate(&appState.platformState);

		return true;
	}
}