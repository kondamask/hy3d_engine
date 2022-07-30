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

#if _DEBUG
#define RENDERER_DLL(api) "bin\\Debug\\"#api"_renderer.dll"
#else
#define RENDERER_DLL(api) "bin\\Release\\"#api"_renderer.dll"
#endif

#define RENDERER_FUNCTION(api, func) api##func
	void RendererLoadCode(renderer* renderer, RENDERER_API rendererAPI)
	{
		bool loadedDLL = false;
		switch (rendererAPI)
		{
		case RENDERER_API_VULKAN: loadedDLL = PlatformLoadDynamicLibrary(RENDERER_DLL(vulkan), &renderer->library); break;
		case RENDERER_API_OPENGL: break;
		case RENDERER_API_DIRECTX: break;
		}

		if (loadedDLL)
		{
			renderer->Initialize =
				(pfnRendererInitialize)PlatformGetDynamicLibraryFunction(&renderer->library, "VulkanInitialize");

			renderer->DrawFrame =
				(pfnRendererDrawFrame)PlatformGetDynamicLibraryFunction(&renderer->library, "VulkanDrawFrame");

			renderer->Terminate =
				(pfnRendererTerminate)PlatformGetDynamicLibraryFunction(&renderer->library, "VulkanTerminate");
		}
	}
} // namespace HY3D
