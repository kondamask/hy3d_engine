#include "renderer/renderer.h"
#include "core/logger.h"

#include "vulkan_renderer.h"
#include "vulkan_functions.cpp"
#include "vulkan_backend.cpp"

#include "core/assert.h"

namespace HY3D
{
	namespace Vulkan
	{
		extern "C" HY3D_API RendererInitializeSignature(RendererInitialize)
		{
			bool result = PlatformLoadSystemLibrary(VULKAN_DLL, &context.library);
			ASSERT(result == true);

			vkGetInstanceProcAddr = (pfn_vkGetInstanceProcAddr)PlatformGetLibraryFunction(&context.library, "vkGetInstanceProcAddr");
			ASSERT(vkGetInstanceProcAddr != 0);

			result = LoadGlobalFunctions();
			ASSERT(result == true);

			result = CreateInstance(platformState);
			ASSERT(result == true);

			result = PickGPU();
			ASSERT(result == true);

			result = CreateSurface(platformState);
			ASSERT(result);

			result = PickCommandQueues();
			ASSERT(result);

			result = CreateDevice();
			ASSERT(result == true);

			result = CreateSwapchain();
			ASSERT(result == true);

			LOG_DEBUG(__FUNCTION__);
			return true;
		}

		extern "C" HY3D_API RendererDrawFrameSignature(RendererDrawFrame)
		{				
			return;
		}

		extern "C" HY3D_API RendererTerminateSignature(RendererTerminate)
		{
			PlatformUnloadLibrary(&context.library);
			LOG_DEBUG(__FUNCTION__);
		}
	} // namespace Vulkan
} // namespace HY3D
