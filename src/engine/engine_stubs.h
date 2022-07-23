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

	void EngineValidateAPI(engine *engine)
	{
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