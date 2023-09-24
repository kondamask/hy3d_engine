#include "renderer.h"
#include "core/logger.h"

namespace HY3D
{

#include "renderer_api_auto.cpp"

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

		if (loadedDLL)
			renderer->api = renderer_api_get(renderer->library);
		else
			renderer->api = renderer_api_get_stubs();

		renderer->api.RendererSetApiContext(&renderer->apiContext);
		renderer->api.RendererOnReload();
	}
} // namespace HY3D
