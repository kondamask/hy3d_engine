#include "renderer/renderer.h"
#include "core/logger.h"

#include "opengl_renderer.h"

namespace HY3D
{
	namespace OpenGL
	{
		extern "C" HY3D_API bool RendererInitialize(platform_state *platformState)
		{
			return false;
		}

		extern "C" HY3D_API void RendererDrawFrame(render_packet *packet)
		{
			return;
		}

		extern "C" HY3D_API void RendererTerminate(void** apiContext)
		{
			return;
		}

		extern "C" HY3D_API void RendererReloadShaders()
		{
			return;
		}

		extern "C" HY3D_API void RendererSetApiContext(void** apiContext)
		{
			return;
		}

		extern "C" HY3D_API void RendererOnReload()
		{
			return;
		}

		
	} // namespace OpenGL
	
} // namespace HY3D
