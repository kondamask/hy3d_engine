#ifndef INCLUDE_APPLICATION_H
#define INCLUDE_APPLICATION_H

#include "defines.h"
#include "platform/platform.h"
#include "engine/engine.h"

namespace HY3D
{
	struct application_config
	{
		i32 width;
		i32 height;
		const char *name;
	};

	struct application_state
	{
		engine engine;
		bool isInitialized;
		bool isSuspended;
		i32 width;
		i32 height;
		platform_state platformState;
	};

	namespace Application
	{
		global_var application_state state;
	}

	bool ApplicationInitialize(application_config *appInfo);
	bool ApplicationRun();
}
#endif // INCLUDE_APPLICATION_H