#include "engine.h"
#include "core/logger.h"

namespace HY3D
{
	EngineInitializeSignature(EngineInitialize)
	{
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