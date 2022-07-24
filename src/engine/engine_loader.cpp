#include "engine.h"
#include "core/logger.h"

namespace HY3D
{
	EngineInitializeSignature(EngineInitializeSTUB)
	{
		LOG_DEBUG("Called Stub Function '%s'", __FUNCTION__);
	}

	EngineUpdateSignature(EngineUpdateSTUB)
	{
		LOG_DEBUG("Called Stub Function '%s'", __FUNCTION__);
	}

	EngineRenderSignature(EngineRenderSTUB)
	{
		LOG_DEBUG("Called Stub Function '%s'", __FUNCTION__);
	}

	EngineTerminateSignature(EngineTerminateSTUB)
	{
		LOG_DEBUG("Called Stub Function '%s'", __FUNCTION__);
	}

#if _DEBUG
#define ENGINE_DLL "bin\\Debug\\engine.dll"
#else
#define ENGINE_DLL "bin\\Release\\engine.dll"
#endif
	void EngineLoadCode(engine *engine, dynamic_library *lib)
	{
		if (PlatformLoadDynamicLibrary(ENGINE_DLL, lib))
		{
			engine->Initialize =
				(pfnEngineInitialize)PlatformGetDynamicLibraryFunction(lib, "EngineInitialize");

			engine->Update =
				(pfnEngineUpdate)PlatformGetDynamicLibraryFunction(lib, "EngineUpdate");

			engine->Render =
				(pfnEngineRender)PlatformGetDynamicLibraryFunction(lib, "EngineRender");

			engine->Terminate =
				(pfnEngineTerminate)PlatformGetDynamicLibraryFunction(lib, "EngineTerminate");
		}

		if (!engine->Initialize)
			engine->Initialize = EngineInitializeSTUB;
		if (!engine->Update)
			engine->Update = EngineUpdateSTUB;
		if (!engine->Render)
			engine->Render = EngineRenderSTUB;
		if (!engine->Terminate)
			engine->Terminate = EngineTerminateSTUB;
	}

	void EngineReloadCode(engine *engine, dynamic_library *lib)
	{
		if (PlatformUpdatedDynamicLibrary(lib))
		{
			PlatformUnloadDynamicLibrary(lib);
			EngineLoadCode(engine, lib);
		}
	}
}