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

	RendererReloadShadersSignature(RendererReloadShadersStub)
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
			loadedDLL = PlatformLoadLibrary(RENDERER_DLL(VULKAN_RENDERER_DLL), &renderer->library); break;
		case RENDERER_API_OPENGL: break;
		case RENDERER_API_DIRECTX: break;
		}

		pfnRendererSetApiContext RendererSetApiContext = 0;
		pfnRendererOnReload RendererOnReload = 0;

		if (loadedDLL)
		{
			renderer->Initialize =
				(pfnRendererInitialize)PlatformGetLibraryFunction(&renderer->library, "RendererInitialize");

			renderer->DrawFrame =
				(pfnRendererDrawFrame)PlatformGetLibraryFunction(&renderer->library, "RendererDrawFrame");

			renderer->ReloadShaders =
				(pfnRendererReloadShaders)PlatformGetLibraryFunction(&renderer->library, "RendererReloadShaders");

			renderer->Terminate =
				(pfnRendererTerminate)PlatformGetLibraryFunction(&renderer->library, "RendererTerminate");

			RendererSetApiContext =
				(pfnRendererSetApiContext)PlatformGetLibraryFunction(&renderer->library, "RendererSetApiContext");

			RendererOnReload =
				(pfnRendererOnReload)PlatformGetLibraryFunction(&renderer->library, "RendererOnReload");
		}

		if (!renderer->Initialize)
			renderer->Initialize = RendererInitializeStub;
		if (!renderer->DrawFrame)
			renderer->DrawFrame = RendererDrawFrameStub;
		if (!renderer->ReloadShaders)
			renderer->ReloadShaders = RendererReloadShadersStub;
		if (!renderer->Terminate)
			renderer->Terminate = RendererTerminateStub;
				
		if (RendererSetApiContext)
			RendererSetApiContext(&renderer->apiContext);

		if (RendererOnReload)
			RendererOnReload();
	}
} // namespace HY3D
