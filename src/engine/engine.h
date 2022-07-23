#ifndef INCLUDE_ENGINE_H
#define INCLUDE_ENGINE_H

#include "defines.h"
#include "platform/platform.h"

namespace HY3D
{
	struct engine
	{
		void (*Initialize)(engine *engine);
		void (*Update)();
		void (*Render)();
		void (*Terminate)();

		platform_api platformAPI;
	};

#define EngineInitializeSignature(name) void name(engine *engine)
	extern "C" HY3D_API EngineInitializeSignature(EngineInitialize);
	typedef EngineInitializeSignature(_EngineInitialize);
#define pfnEngineInitialize _EngineInitialize *

#define EngineUpdateSignature(name) void name()
	extern "C" HY3D_API EngineUpdateSignature(EngineUpdate);
	typedef EngineUpdateSignature(_EngineUpdate);
#define pfnEngineUpdate _EngineUpdate *

#define EngineRenderSignature(name) void name()
	extern "C" HY3D_API EngineRenderSignature(EngineRender);
	typedef EngineRenderSignature(_EngineRender);
#define pfnEngineRender _EngineRender *

#define EngineTerminateSignature(name) void name()
	extern "C" HY3D_API EngineTerminateSignature(EngineTerminate);
	typedef EngineTerminateSignature(_EngineTerminate);
#define pfnEngineTerminate _EngineTerminate *
}

#endif // INCLUDE_ENGINE_H