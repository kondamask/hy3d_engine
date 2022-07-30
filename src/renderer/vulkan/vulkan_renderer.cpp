#include "renderer/renderer.h"
#include "core/logger.h"

namespace HY3D
{
	extern "C" HY3D_API RendererInitializeSignature(VulkanInitialize)
	{
		LOG_DEBUG(__FUNCTION__);
		return true;
	}

	extern "C" HY3D_API RendererDrawFrameSignature(VulkanDrawFrame)
	{
		return;
	}

	extern "C" HY3D_API RendererTerminateSignature(VulkanTerminate)
	{
		LOG_DEBUG(__FUNCTION__);
	}
} // namespace HY3D
