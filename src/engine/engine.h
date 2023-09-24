#ifndef INCLUDE_ENGINE_H
#define INCLUDE_ENGINE_H

#include "defines.h"
#include "platform/platform.h"

namespace HY3D
{
#include "engine_api_auto.h"

	struct engine
	{
		dynamic_library library;
		engine_api      api;
	};
}

#endif // INCLUDE_ENGINE_H