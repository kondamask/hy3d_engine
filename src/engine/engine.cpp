#include "engine.h"

// NOTE: We basicaly compile logger.cpp two times in order to access the function definitions
// from the engine as well. It's just one function so it does not really matter.
#include "core/logger.cpp"

namespace HY3D
{
	namespace Platform
	{
		global_var platform_api *API;
	}

	EngineInitializeSignature(EngineInitialize)
	{
		Platform::API = &engine->platformAPI;

		// NOTE: We are only doing this because the logger uses special Platform specific functions
		// to print to the console, so we need to set the function pointers sinces the engine does
		// not know anything about the platform
		Platform::API->BindToLogger(&Logger::API);

		LOG_DEBUG(__FUNCTION__);
	}

	EngineUpdateSignature(EngineUpdate)
	{
	}

	EngineRenderSignature(EngineRender)
	{
	}

	EngineTerminateSignature(EngineTerminate)
	{
		LOG_DEBUG(__FUNCTION__);
	}
}