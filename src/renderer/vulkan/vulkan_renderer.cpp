#include "renderer/renderer.h"
#include "defines.h"
#include "core/logger.h"

#if PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"

namespace HY3D
{
	namespace Vulkan
	{
		extern "C" HY3D_API RendererInitializeSignature(RendererInitialize)
		{
			LOG_DEBUG(__FUNCTION__);
			return true;
		}

		extern "C" HY3D_API RendererDrawFrameSignature(RendererDrawFrame)
		{
			return;
		}

		extern "C" HY3D_API RendererTerminateSignature(RendererTerminate)
		{
			LOG_DEBUG(__FUNCTION__);
		}
	} // namespace Vulkan
} // namespace HY3D
