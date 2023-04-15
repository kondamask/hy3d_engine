#ifndef INCLUDE_RENDERER_H
#define INCLUDE_RENDERER_H

#include "platform/platform.h"

// NOTE: We have a main renderer api that the application can use
// We will export these functions and link to them dynamically from the
// renderer dll.
// Each Graphics API backend needs to implement these exposes functions
// using the same name so that we can load them

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

#define RendererInitializeSignature(name) bool name(platform_state *platformState)
	typedef RendererInitializeSignature(_RendererInitialize);
#define pfnRendererInitialize _RendererInitialize*

#define RendererOnReloadSignature(name) void name()
	typedef RendererOnReloadSignature(_RendererOnReload);
#define pfnRendererOnReload _RendererOnReload*

#define RendererSetApiContextSignature(name) void name(void** apiContext)
	typedef RendererSetApiContextSignature(_RendererSetApiContext);
#define pfnRendererSetApiContext _RendererSetApiContext*

#define RendererDrawFrameSignature(name) void name(render_packet *packet)
	typedef RendererDrawFrameSignature(_RendererDrawFrame);
#define pfnRendererDrawFrame _RendererDrawFrame*

#define RendererReloadShadersSignature(name) void name()
	typedef RendererReloadShadersSignature(_RendererReloadShaders);
#define pfnRendererReloadShaders _RendererReloadShaders*

#define RendererTerminateSignature(name) void name()
	typedef RendererTerminateSignature(_RendererTerminate);
#define pfnRendererTerminate _RendererTerminate*

	struct renderer
	{
		dynamic_library library;
		file_write_time shadersWriteTime;

		pfnRendererInitialize Initialize;
		pfnRendererDrawFrame DrawFrame;
		pfnRendererReloadShaders ReloadShaders;
		pfnRendererTerminate Terminate;

		void* apiContext;
	};

} // namespace HY3D

#endif // INCLUDE_RENDERER_H