#ifndef INCLUDE_ENGINE_H
#define INCLUDE_ENGINE_H

#include "defines.h"

namespace HY3D
{
	struct engine
	{
		void (*Initialize)();
		void (*Update)();
		void (*Render)();
		void (*Terminate)();
	};

#define EngineInitializeSignature(name) void name()
	typedef EngineInitializeSignature(_EngineInitialize);
#define pfnEngineInitialize _EngineInitialize *
	EngineInitializeSignature(EngineInitializeSTUB)
	{
		return;
	}
	extern "C" HY3D_API void EngineInitialize();

#define EngineUpdateSignature(name) void name()
	typedef EngineUpdateSignature(_EngineUpdate);
#define pfnEngineUpdate _EngineUpdate *
	EngineUpdateSignature(EngineUpdateSTUB)
	{
		return;
	}
	extern "C" HY3D_API void EngineUpdate();

#define EngineRenderSignature(name) void name()
	typedef EngineRenderSignature(_EngineRender);
#define pfnEngineRender _EngineRender *
	EngineRenderSignature(EngineRenderSTUB)
	{
		return;
	}
	extern "C" HY3D_API void EngineRender();

#define EngineTerminateSignature(name) void name()
	typedef EngineTerminateSignature(_EngineTerminate);
#define pfnEngineTerminate _EngineTerminate *
	EngineTerminateSignature(EngineTerminateSTUB)
	{
		return;
	}
	extern "C" HY3D_API void EngineTerminate();
}

#endif // INCLUDE_ENGINE_H