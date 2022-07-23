#ifndef INCLUDE_ENGINE_H
#define INCLUDE_ENGINE_H

#include "defines.h"
#include "platform/platform.h"

namespace HY3D
{
	global_var platform_api *platformAPI;

	struct engine
	{
		void (*Initialize)(engine *engine);
		void (*Update)();
		void (*Render)();
		void (*Terminate)();

		platform_api platformAPI;
	};

#define EngineInitializeSignature(name) void name(engine *engine)
	typedef EngineInitializeSignature(_EngineInitialize);
	extern "C" HY3D_API EngineInitializeSignature(EngineInitialize);
#define pfnEngineInitialize _EngineInitialize *

#define EngineUpdateSignature(name) void name()
	typedef EngineUpdateSignature(_EngineUpdate);
	extern "C" HY3D_API EngineUpdateSignature(EngineUpdate);
#define pfnEngineUpdate _EngineUpdate *

#define EngineRenderSignature(name) void name()
	typedef EngineRenderSignature(_EngineRender);
	extern "C" HY3D_API EngineRenderSignature(EngineRender);
#define pfnEngineRender _EngineRender *

#define EngineTerminateSignature(name) void name()
	typedef EngineTerminateSignature(_EngineTerminate);
	extern "C" HY3D_API EngineTerminateSignature(EngineTerminate);
#define pfnEngineTerminate _EngineTerminate *
}

#endif // INCLUDE_ENGINE_H