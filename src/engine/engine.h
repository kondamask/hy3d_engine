#ifndef INCLUDE_ENGINE_H
#define INCLUDE_ENGINE_H

#include "defines.h"
#include "platform/platform.h"

namespace HY3D
{
#define EngineInitializeSignature(name) bool name()
	typedef EngineInitializeSignature(_EngineInitialize);
#define pfnEngineInitialize _EngineInitialize *

#define EngineUpdateSignature(name) void name(f64 dt)
	extern "C" HY3D_API EngineUpdateSignature(EngineUpdate);
	typedef EngineUpdateSignature(_EngineUpdate);
#define pfnEngineUpdate _EngineUpdate *

#define EngineRenderSignature(name) void name(f64 dt)
	typedef EngineRenderSignature(_EngineRender);
#define pfnEngineRender _EngineRender *

#define EngineTerminateSignature(name) void name()
	typedef EngineTerminateSignature(_EngineTerminate);
#define pfnEngineTerminate _EngineTerminate *

	struct engine
	{
		dynamic_library library;

		pfnEngineInitialize Initialize;
		pfnEngineUpdate Update;
		pfnEngineRender Render;
		pfnEngineTerminate Terminate;
	};
}

#endif // INCLUDE_ENGINE_H