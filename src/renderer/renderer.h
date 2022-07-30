#ifndef INCLUDE_RENDERER_H
#define INCLUDE_RENDERER_H

#include "platform/platform.h"

namespace HY3D
{
	enum RENDERER_API
	{
		RENDERER_API_VULKAN,
		RENDERER_API_OPENGL,
		RENDERER_API_DIRECTX,
	};

	
	struct render_packet
	{
		f64 dt;
	};

#define RendererInitializeSignature(name) bool name()
	typedef RendererInitializeSignature(_RendererInitialize);
#define pfnRendererInitialize _RendererInitialize *

#define RendererDrawFrameSignature(name) void name(render_packet *packet)
	typedef RendererDrawFrameSignature(_RendererDrawFrame);
#define pfnRendererDrawFrame _RendererDrawFrame *

#define RendererTerminateSignature(name) void name()
	typedef RendererTerminateSignature(_RendererTerminate);
#define pfnRendererTerminate _RendererTerminate *

	struct renderer
	{
		dynamic_library library;

		pfnRendererInitialize Initialize;
		pfnRendererDrawFrame DrawFrame;
		pfnRendererTerminate Terminate;
	};

} // namespace HY3D

#endif // INCLUDE_RENDERER_H