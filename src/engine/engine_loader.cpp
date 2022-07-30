#include "engine.h"
#include "core/logger.h"

namespace HY3D
{
	EngineInitializeSignature(EngineInitializeSTUB)
	{
		LOG_DEBUG("Called Stub Function '%s'", __FUNCTION__);
		return false;
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
	void EngineLoadCode(engine *engine)
	{
		if (PlatformLoadDynamicLibrary(ENGINE_DLL, &engine->library))
		{
			engine->Initialize =
				(pfnEngineInitialize)PlatformGetDynamicLibraryFunction(&engine->library, "EngineInitialize");

			engine->Update =
				(pfnEngineUpdate)PlatformGetDynamicLibraryFunction(&engine->library, "EngineUpdate");

			engine->Render =
				(pfnEngineRender)PlatformGetDynamicLibraryFunction(&engine->library, "EngineRender");

			engine->Terminate =
				(pfnEngineTerminate)PlatformGetDynamicLibraryFunction(&engine->library, "EngineTerminate");
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
}