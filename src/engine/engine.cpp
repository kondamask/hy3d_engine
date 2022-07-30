#include "engine.h"
#include "core/logger.h"

namespace HY3D
{
	extern "C" HY3D_API EngineInitializeSignature(EngineInitialize)
	{
		LOG_DEBUG(__FUNCTION__);
		return true;
	}

	extern "C" HY3D_API EngineUpdateSignature(EngineUpdate)
	{
	}

	extern "C" HY3D_API EngineRenderSignature(EngineRender)
	{
	}

	extern "C" HY3D_API EngineTerminateSignature(EngineTerminate)
	{
		LOG_DEBUG(__FUNCTION__);
	}
}