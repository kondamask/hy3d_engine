#ifndef INCLUDE_VULKAN_FUNCTIONS_CPP
#define INCLUDE_VULKAN_FUNCTIONS_CPP

#include "defines.h"

namespace HY3D
{
	namespace Vulkan
	{
#define VulkanFuncPtr(func) PFN_##func
#define VulkanDeclareFunction(func) static_func VulkanFuncPtr(func) func

		/////////////////////////////////////////////////////////////////////////////////////////
		// Global Functions
		/////////////////////////////////////////////////////////////////////////////////////////
#define vkGetInstanceProcAddrSignature(name) PFN_vkVoidFunction name(VkInstance instance, const char *pName)
		typedef vkGetInstanceProcAddrSignature(_vkGetInstanceProcAddr);
#define pfn_vkGetInstanceProcAddr _vkGetInstanceProcAddr*
		static_func pfn_vkGetInstanceProcAddr vkGetInstanceProcAddr_;
#define vkGetInstanceProcAddr vkGetInstanceProcAddr_

		VulkanDeclareFunction(vkCreateInstance);
		VulkanDeclareFunction(vkEnumerateInstanceLayerProperties);
		VulkanDeclareFunction(vkEnumerateInstanceExtensionProperties);

#define VulkanLoadGlobalFunc(func)									   \
	func = (VulkanFuncPtr(func))vkGetInstanceProcAddr(nullptr, #func); \
	if (!(func)) { return false; }

		static_func bool LoadGlobalFunctions()
		{
			VulkanLoadGlobalFunc(vkCreateInstance);
			VulkanLoadGlobalFunc(vkEnumerateInstanceLayerProperties);
			VulkanLoadGlobalFunc(vkEnumerateInstanceExtensionProperties);

			LOG_DEBUG(__FUNCTION__);
			return true;
		}

		//------------------------------------------------------------------------
		// Instance Functions         
		//------------------------------------------------------------------------
		VulkanDeclareFunction(vkDestroyInstance);

#if _DEBUG
		VulkanDeclareFunction(vkCreateDebugUtilsMessengerEXT);
		VulkanDeclareFunction(vkDestroyDebugUtilsMessengerEXT);
#endif

		VulkanDeclareFunction(vkCreateWin32SurfaceKHR); // NOTE: Windows ONLY
		VulkanDeclareFunction(vkDestroySurfaceKHR);
		VulkanDeclareFunction(vkEnumeratePhysicalDevices);
		VulkanDeclareFunction(vkGetPhysicalDeviceQueueFamilyProperties);
		VulkanDeclareFunction(vkGetPhysicalDeviceSurfaceSupportKHR);
		VulkanDeclareFunction(vkGetPhysicalDeviceSurfaceFormatsKHR);
		VulkanDeclareFunction(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
		VulkanDeclareFunction(vkGetPhysicalDeviceFormatProperties);
		VulkanDeclareFunction(vkGetPhysicalDeviceMemoryProperties);
		VulkanDeclareFunction(vkGetPhysicalDeviceProperties);
		VulkanDeclareFunction(vkGetPhysicalDeviceSurfacePresentModesKHR);
		VulkanDeclareFunction(vkEnumerateDeviceExtensionProperties);
		VulkanDeclareFunction(vkCreateDevice);
		VulkanDeclareFunction(vkGetDeviceProcAddr);

#define VulkanLoadInstanceFunc(func)                                                  \
	func = (VulkanFuncPtr(func))vkGetInstanceProcAddr(context->instance, #func); \
	if (!(func)) { return false; }

		static_func bool LoadInstanceFunctions()
		{
			VulkanLoadInstanceFunc(vkDestroyInstance);

#if _DEBUG
			VulkanLoadInstanceFunc(vkCreateDebugUtilsMessengerEXT);
			VulkanLoadInstanceFunc(vkDestroyDebugUtilsMessengerEXT);
#endif

#if PLATFORM_WINDOWS
			VulkanLoadInstanceFunc(vkCreateWin32SurfaceKHR);
#endif

			VulkanLoadInstanceFunc(vkDestroySurfaceKHR);
			VulkanLoadInstanceFunc(vkEnumeratePhysicalDevices);
			VulkanLoadInstanceFunc(vkGetPhysicalDeviceQueueFamilyProperties);
			VulkanLoadInstanceFunc(vkGetPhysicalDeviceSurfaceSupportKHR);
			VulkanLoadInstanceFunc(vkGetPhysicalDeviceSurfaceFormatsKHR);
			VulkanLoadInstanceFunc(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
			VulkanLoadInstanceFunc(vkGetPhysicalDeviceFormatProperties);
			VulkanLoadInstanceFunc(vkGetPhysicalDeviceMemoryProperties);
			VulkanLoadInstanceFunc(vkGetPhysicalDeviceProperties);
			VulkanLoadInstanceFunc(vkGetPhysicalDeviceSurfacePresentModesKHR);
			VulkanLoadInstanceFunc(vkEnumerateDeviceExtensionProperties);
			VulkanLoadInstanceFunc(vkCreateDevice);
			VulkanLoadInstanceFunc(vkGetDeviceProcAddr);

			LOG_DEBUG(__FUNCTION__);
			return true;
		}

		//------------------------------------------------------------------------
		// Device Functions         
		//------------------------------------------------------------------------
		VulkanDeclareFunction(vkDeviceWaitIdle);
		VulkanDeclareFunction(vkDestroyDevice);
		VulkanDeclareFunction(vkGetDeviceQueue);
		VulkanDeclareFunction(vkCreateSwapchainKHR);
		VulkanDeclareFunction(vkDestroySwapchainKHR);
		VulkanDeclareFunction(vkGetSwapchainImagesKHR);
		VulkanDeclareFunction(vkCreateSemaphore);
		VulkanDeclareFunction(vkDestroySemaphore);
		VulkanDeclareFunction(vkAcquireNextImageKHR);
		VulkanDeclareFunction(vkQueueSubmit);
		VulkanDeclareFunction(vkQueuePresentKHR);
		VulkanDeclareFunction(vkCreateCommandPool);
		VulkanDeclareFunction(vkResetCommandPool);
		VulkanDeclareFunction(vkDestroyCommandPool);
		VulkanDeclareFunction(vkAllocateCommandBuffers);
		VulkanDeclareFunction(vkFreeCommandBuffers);
		VulkanDeclareFunction(vkBeginCommandBuffer);
		VulkanDeclareFunction(vkEndCommandBuffer);
		VulkanDeclareFunction(vkResetCommandBuffer);
		VulkanDeclareFunction(vkCreateImage);
		VulkanDeclareFunction(vkDestroyImage);
		VulkanDeclareFunction(vkCreateImageView);
		VulkanDeclareFunction(vkDestroyImageView);
		VulkanDeclareFunction(vkGetImageMemoryRequirements);
		VulkanDeclareFunction(vkBindImageMemory);
		VulkanDeclareFunction(vkAllocateMemory);
		VulkanDeclareFunction(vkFreeMemory);
		VulkanDeclareFunction(vkCreateRenderPass);
		VulkanDeclareFunction(vkDestroyRenderPass);
		VulkanDeclareFunction(vkCreateFramebuffer);
		VulkanDeclareFunction(vkDestroyFramebuffer);
		VulkanDeclareFunction(vkCreateFence);
		VulkanDeclareFunction(vkResetFences);
		VulkanDeclareFunction(vkDestroyFence);
		VulkanDeclareFunction(vkWaitForFences);
		VulkanDeclareFunction(vkCreateShaderModule);
		VulkanDeclareFunction(vkDestroyShaderModule);
		VulkanDeclareFunction(vkCreatePipelineLayout);
		VulkanDeclareFunction(vkDestroyPipelineLayout);
		VulkanDeclareFunction(vkCreateGraphicsPipelines);
		VulkanDeclareFunction(vkDestroyPipeline);
		VulkanDeclareFunction(vkCreateBuffer);
		VulkanDeclareFunction(vkDestroyBuffer);
		VulkanDeclareFunction(vkGetBufferMemoryRequirements);
		VulkanDeclareFunction(vkBindBufferMemory);
		VulkanDeclareFunction(vkMapMemory);
		VulkanDeclareFunction(vkUnmapMemory);
		VulkanDeclareFunction(vkFlushMappedMemoryRanges);
		VulkanDeclareFunction(vkCreateDescriptorSetLayout);
		VulkanDeclareFunction(vkDestroyDescriptorSetLayout);
		VulkanDeclareFunction(vkCreateDescriptorPool);
		VulkanDeclareFunction(vkDestroyDescriptorPool);
		VulkanDeclareFunction(vkAllocateDescriptorSets);
		VulkanDeclareFunction(vkUpdateDescriptorSets);
		VulkanDeclareFunction(vkCreateSampler);
		VulkanDeclareFunction(vkDestroySampler);

		VulkanDeclareFunction(vkCmdPipelineBarrier);
		VulkanDeclareFunction(vkCmdClearColorImage);
		VulkanDeclareFunction(vkCmdEndRenderPass);
		VulkanDeclareFunction(vkCmdBeginRenderPass);
		VulkanDeclareFunction(vkCmdBindPipeline);
		VulkanDeclareFunction(vkCmdBindVertexBuffers);
		VulkanDeclareFunction(vkCmdBindIndexBuffer);
		VulkanDeclareFunction(vkCmdDraw);
		VulkanDeclareFunction(vkCmdDrawIndexed);
		VulkanDeclareFunction(vkCmdSetViewport);
		VulkanDeclareFunction(vkCmdSetScissor);
		VulkanDeclareFunction(vkCmdCopyBuffer);
		VulkanDeclareFunction(vkCmdBindDescriptorSets);
		VulkanDeclareFunction(vkCmdCopyBufferToImage);
		VulkanDeclareFunction(vkCmdPushConstants);
		VulkanDeclareFunction(vkCmdBlitImage);

#define VulkanLoadDeviceFunc(func) 											  	   \
	func = (VulkanFuncPtr(func))vkGetDeviceProcAddr(context->device, #func); \
	if (!(func)) { return false; }

		static_func bool LoadDeviceFunctions()
		{
			VulkanLoadDeviceFunc(vkDeviceWaitIdle);
			VulkanLoadDeviceFunc(vkDestroyDevice);
			VulkanLoadDeviceFunc(vkGetDeviceQueue);
			VulkanLoadDeviceFunc(vkCreateSwapchainKHR);
			VulkanLoadDeviceFunc(vkDestroySwapchainKHR);
			VulkanLoadDeviceFunc(vkGetSwapchainImagesKHR);
			VulkanLoadDeviceFunc(vkCreateSemaphore);
			VulkanLoadDeviceFunc(vkDestroySemaphore);
			VulkanLoadDeviceFunc(vkAcquireNextImageKHR);
			VulkanLoadDeviceFunc(vkQueueSubmit);
			VulkanLoadDeviceFunc(vkQueuePresentKHR);
			VulkanLoadDeviceFunc(vkCreateCommandPool);
			VulkanLoadDeviceFunc(vkResetCommandPool);
			VulkanLoadDeviceFunc(vkDestroyCommandPool);
			VulkanLoadDeviceFunc(vkAllocateCommandBuffers);
			VulkanLoadDeviceFunc(vkFreeCommandBuffers);
			VulkanLoadDeviceFunc(vkBeginCommandBuffer);
			VulkanLoadDeviceFunc(vkEndCommandBuffer);
			VulkanLoadDeviceFunc(vkResetCommandBuffer);
			VulkanLoadDeviceFunc(vkCreateImage);
			VulkanLoadDeviceFunc(vkDestroyImage);
			VulkanLoadDeviceFunc(vkCreateImageView);
			VulkanLoadDeviceFunc(vkDestroyImageView);
			VulkanLoadDeviceFunc(vkGetImageMemoryRequirements);
			VulkanLoadDeviceFunc(vkBindImageMemory);
			VulkanLoadDeviceFunc(vkAllocateMemory);
			VulkanLoadDeviceFunc(vkFreeMemory);
			VulkanLoadDeviceFunc(vkCreateRenderPass);
			VulkanLoadDeviceFunc(vkDestroyRenderPass);
			VulkanLoadDeviceFunc(vkCreateFramebuffer);
			VulkanLoadDeviceFunc(vkDestroyFramebuffer);
			VulkanLoadDeviceFunc(vkCreateFence);
			VulkanLoadDeviceFunc(vkDestroyFence);
			VulkanLoadDeviceFunc(vkWaitForFences);
			VulkanLoadDeviceFunc(vkResetFences);
			VulkanLoadDeviceFunc(vkCreateShaderModule);
			VulkanLoadDeviceFunc(vkDestroyShaderModule);
			VulkanLoadDeviceFunc(vkCreatePipelineLayout);
			VulkanLoadDeviceFunc(vkDestroyPipelineLayout);
			VulkanLoadDeviceFunc(vkCreateGraphicsPipelines);
			VulkanLoadDeviceFunc(vkDestroyPipeline);
			VulkanLoadDeviceFunc(vkCreateBuffer);
			VulkanLoadDeviceFunc(vkDestroyBuffer);
			VulkanLoadDeviceFunc(vkBindBufferMemory);
			VulkanLoadDeviceFunc(vkMapMemory);
			VulkanLoadDeviceFunc(vkUnmapMemory);
			VulkanLoadDeviceFunc(vkGetBufferMemoryRequirements);
			VulkanLoadDeviceFunc(vkFlushMappedMemoryRanges);
			VulkanLoadDeviceFunc(vkCreateDescriptorSetLayout);
			VulkanLoadDeviceFunc(vkDestroyDescriptorSetLayout);
			VulkanLoadDeviceFunc(vkCreateDescriptorPool);
			VulkanLoadDeviceFunc(vkDestroyDescriptorPool);
			VulkanLoadDeviceFunc(vkAllocateDescriptorSets);
			VulkanLoadDeviceFunc(vkUpdateDescriptorSets);
			VulkanLoadDeviceFunc(vkCreateSampler);
			VulkanLoadDeviceFunc(vkDestroySampler);

			VulkanLoadDeviceFunc(vkCmdBeginRenderPass);
			VulkanLoadDeviceFunc(vkCmdEndRenderPass);
			VulkanLoadDeviceFunc(vkCmdPipelineBarrier);
			VulkanLoadDeviceFunc(vkCmdClearColorImage);
			VulkanLoadDeviceFunc(vkCmdBindPipeline);
			VulkanLoadDeviceFunc(vkCmdBindVertexBuffers);
			VulkanLoadDeviceFunc(vkCmdBindIndexBuffer);
			VulkanLoadDeviceFunc(vkCmdDraw);
			VulkanLoadDeviceFunc(vkCmdDrawIndexed);
			VulkanLoadDeviceFunc(vkCmdSetViewport);
			VulkanLoadDeviceFunc(vkCmdSetScissor);
			VulkanLoadDeviceFunc(vkCmdCopyBuffer);
			VulkanLoadDeviceFunc(vkCmdBindDescriptorSets);
			VulkanLoadDeviceFunc(vkCmdCopyBufferToImage);
			VulkanLoadDeviceFunc(vkCmdPushConstants);
			VulkanLoadDeviceFunc(vkCmdBlitImage);

			LOG_DEBUG(__FUNCTION__);
			return true;
		}
	} // namespace Vulkan
} // namespace HY3D

#endif // INCLUDE_VULKAN_FUNCTIONS_CPP
