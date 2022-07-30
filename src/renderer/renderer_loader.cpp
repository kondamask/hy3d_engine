#include "renderer.h"
#include "core/logger.h"

namespace HY3D
{

	RendererInitializeSignature(RendererInitializeStub)
	{
		LOG_DEBUG("Called Stub Function '%s'", __FUNCTION__);
		return false;
	}

	RendererDrawFrameSignature(RendererDrawFrameStub)
	{
		LOG_DEBUG("Called Stub Function '%s'", __FUNCTION__);
	}

	RendererTerminateSignature(RendererTerminateStub)
	{
		LOG_DEBUG("Called Stub Function '%s'", __FUNCTION__);
	}

#define VULKAN_RENDERER_DLL "vulkan_renderer.dll"
#if _DEBUG
#define RENDERER_DLL(api) "bin\\Debug\\"##api
#else
#define RENDERER_DLL(api) "bin\\Release\\"##api
#endif
	void RendererLoadCode(renderer* renderer, RENDERER_API rendererAPI)
	{
		bool loadedDLL = false;
		switch (rendererAPI)
		{
		case RENDERER_API_VULKAN:
			loadedDLL = PlatformLoadDynamicLibrary(RENDERER_DLL(VULKAN_RENDERER_DLL), &renderer->library); break;
		case RENDERER_API_OPENGL: break;
		case RENDERER_API_DIRECTX: break;
		}

		if (loadedDLL)
		{
			renderer->Initialize =
				(pfnRendererInitialize)PlatformGetDynamicLibraryFunction(&renderer->library, "RendererInitialize");

			renderer->DrawFrame =
				(pfnRendererDrawFrame)PlatformGetDynamicLibraryFunction(&renderer->library, "RendererDrawFrame");

			renderer->Terminate =
				(pfnRendererTerminate)PlatformGetDynamicLibraryFunction(&renderer->library, "RendererTerminate");
		}

		if (!renderer->Initialize)
			renderer->Initialize = RendererInitializeStub;
		if (!renderer->DrawFrame)
			renderer->DrawFrame = RendererDrawFrameStub;
		if (!renderer->Terminate)
			renderer->Terminate = RendererTerminateStub;
	}
} // namespace HY3D
