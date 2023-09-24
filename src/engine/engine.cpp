#include "engine.h"
#include "core/logger.h"
#include "core/input.h"
#include "renderer/renderer.h"

namespace HY3D
{
	namespace Engine
	{
		extern "C" HY3D_API bool EngineInitialize()
		{
			LOG_INFO(__FUNCTION__);
			return true;
		}

		extern "C" HY3D_API void EngineUpdate(f64 dt)
		{
		}

		extern "C" HY3D_API void EngineRender(f64 dt)
		{
		}

		extern "C" HY3D_API void EngineTerminate()
		{
			LOG_INFO(__FUNCTION__);
		}
	}
}