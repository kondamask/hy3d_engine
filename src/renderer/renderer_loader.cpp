#include "renderer.h"
#include "core/logger.h"

namespace HY3D
{

#include "renderer_api_auto.cpp"

#define VULKAN_RENDERER_DLL "vulkan_renderer.dll"
#define OPENGL_RENDERER_DLL "opengl_renderer.dll"
#if _DEBUG
#define RENDERER_DLL(api) "bin\\Debug\\"##api
#else
#define RENDERER_DLL(api) "bin\\Release\\"##api
#endif
	void RendererLoadCode(renderer* renderer, renderer_graphics_api rendererAPI)
	{
		bool loadedDLL = false;
		switch (rendererAPI)
		{
		case RENDERER_GRAPHICS_API_VULKAN:
			renderer->graphics_api = RENDERER_GRAPHICS_API_VULKAN;
			loadedDLL = PlatformLoadLibrary(RENDERER_DLL(VULKAN_RENDERER_DLL), &renderer->library); break;
		case RENDERER_GRAPHICS_API_OPENGL:
			renderer->graphics_api = RENDERER_GRAPHICS_API_OPENGL;
			loadedDLL = PlatformLoadLibrary(RENDERER_DLL(OPENGL_RENDERER_DLL), &renderer->library); break;
		case RENDERER_GRAPHICS_API_DIRECTX:
			break;
		}

		if (loadedDLL)
			renderer->api = renderer_api_get(renderer->library);
		else
			renderer->api = renderer_api_get_stubs();

		renderer->api.RendererSetApiContext(&renderer->apiContext);
		renderer->api.RendererOnReload();
	}
} // namespace HY3D
