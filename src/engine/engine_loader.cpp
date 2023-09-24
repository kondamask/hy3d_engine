#include "engine.h"
#include "core/logger.h"

namespace HY3D
{
#include "engine_api_auto.cpp"

#if _DEBUG
#define ENGINE_DLL "bin\\Debug\\engine.dll"
#else
#define ENGINE_DLL "bin\\Release\\engine.dll"
#endif
	void EngineLoadCode(engine *engine)
	{
		if (PlatformLoadLibrary(ENGINE_DLL, &engine->library))
			engine->api = engine_api_get(engine->library);
		else
			engine->api = engine_api_get_stubs();
	}
}