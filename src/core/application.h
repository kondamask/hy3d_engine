#ifndef INCLUDE_APPLICATION_H
#define INCLUDE_APPLICATION_H

#include "defines.h"
#include "platform/platform.h"

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
		bool isInitialized;
		bool isSuspended;
		i32 width;
		i32 height;
		platform_state platformState;
	};
	global_var application_state appState;

	static_func bool ApplicationInitialize(application_config *appInfo);
	static_func bool ApplicationRun();
}
#endif // INCLUDE_APPLICATION_H