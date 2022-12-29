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
			bool result = PlatformLoadSystemLibrary(VULKAN_DLL, &context->library);
			ASSERT(result == true);

			vkGetInstanceProcAddr = (pfn_vkGetInstanceProcAddr)PlatformGetLibraryFunction(&context->library, "vkGetInstanceProcAddr");
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

			result = CreateCommandResources();
			ASSERT(result == true);

			result = CreateRenderPass();
			ASSERT(result == true);

			result = CreateSwapchain();
			ASSERT(result == true);

			LOG_INFO(__FUNCTION__);

			context->isInitialized = true;
			return true;
		}

		extern "C" HY3D_API RendererDrawFrameSignature(RendererDrawFrame)
		{
			cmd_resources* res = GetNextAvailableCommandResource();

			// NOTE: Get the next image that we'll use to create the frame and draw it
			// Signal the semaphore when it's available
			local_var u32 nextImage = 0;
			local_var VkResult result = {};
			{
				result = vkAcquireNextImageKHR(context->device, context->swapchain, UINT64_MAX, res->imgAvailableSem, 0, &nextImage);

				if (result == VK_ERROR_OUT_OF_DATE_KHR)
				{
					if (!CreateSwapchain())
						return;
				}
				else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
				{
					ASSERT(!(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR));
					return;
				}
			}

			local_var VkCommandBufferBeginInfo commandBufferBeginInfo = {VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
			commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			// NOTE: THE ORDER OF THESE VALUES MUST BE IDENTICAL
			// TO THE ORDER WE SPECIFIED THE RENDERPASS ATTACHMENTS
			local_var VkClearValue clearValues[] = {
				{1.0, 0.0, 0.0, 1.0}
			};

			local_var VkRenderPassBeginInfo renderpassInfo = {VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
			renderpassInfo.renderPass = context->renderPass;
			renderpassInfo.renderArea.extent = context->windowExtent;
			renderpassInfo.clearValueCount = ArrayCount(clearValues);
			renderpassInfo.pClearValues = clearValues;
			renderpassInfo.framebuffer = context->framebuffers[nextImage];
			
			// TODO: VkSuccess
			vkBeginCommandBuffer(res->cmdBuffer, &commandBufferBeginInfo);

			vkCmdBeginRenderPass(res->cmdBuffer, &renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdEndRenderPass(res->cmdBuffer);

			vkEndCommandBuffer(res->cmdBuffer);

			local_var VkPipelineStageFlags waitDestStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			local_var VkSubmitInfo submitInfo = {VK_STRUCTURE_TYPE_SUBMIT_INFO};
			submitInfo.pWaitDstStageMask = &waitDestStageMask;
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = &res->imgAvailableSem;
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &res->frameReadySem;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &res->cmdBuffer;
			vkQueueSubmit(context->graphicsQueue, 1, &submitInfo, res->fence);

			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				bool createSC = CreateSwapchain();
				ASSERT(createSC);
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				ASSERT(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR);
				return;
			}

			local_var VkPresentInfoKHR presentInfo = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &res->frameReadySem;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &context->swapchain;
			presentInfo.pImageIndices = &nextImage;
			result = vkQueuePresentKHR(context->presentQueue, &presentInfo);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				bool createSC = CreateSwapchain();
				ASSERT(createSC);
			}
			else if (result != VK_SUCCESS)
			{
				ASSERT(result == VK_SUCCESS);
				return;
			}

			return;
		}

		extern "C" HY3D_API RendererTerminateSignature(RendererTerminate)
		{
			PlatformUnloadLibrary(&context->library);
			LOG_INFO(__FUNCTION__);
		}

		extern "C" HY3D_API RendererSetApiContextSignature(RendererSetApiContext)
		{
			if (!(*apiContext))
			{
				vulkan_context *ptr = new vulkan_context();
				*apiContext = (void*)(ptr);
			}
			context = ((vulkan_context*)*apiContext);
		}

		extern "C" HY3D_API RendererOnReloadSignature(RendererOnReload)
		{
			if (!context->isInitialized)
				return;

			vkGetInstanceProcAddr = (pfn_vkGetInstanceProcAddr)PlatformGetLibraryFunction(&context->library, "vkGetInstanceProcAddr");
			ASSERT(vkGetInstanceProcAddr != 0);

			LoadGlobalFunctions();
			LoadInstanceFunctions();
			LoadDeviceFunctions();
		}
	} // namespace Vulkan
} // namespace HY3D
