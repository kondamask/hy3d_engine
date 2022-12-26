#ifndef INCLUDE_APPLICATION_H
#define INCLUDE_APPLICATION_H

#include "defines.h"
#include "platform/platform.h"
#include "engine/engine.h"
#include "renderer/renderer.h"

namespace HY3D
{
#define LIMIT_FRAMES 0
#define TARGET_FPS 60.0

	struct application_config
	{
		i32 width;
		i32 height;
		const char* name;
	};

	namespace Application
	{
		global_var struct {
			engine engine;
			renderer renderer;
			bool isInitialized;
			bool isSuspended;
			i32 width;
			i32 height;
			platform_state platformState;
		} state;
	}

	bool ApplicationInitialize(application_config* appInfo);
	bool ApplicationRun();
}
#endif // INCLUDE_APPLICATION_H