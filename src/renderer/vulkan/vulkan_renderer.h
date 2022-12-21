#ifndef INCLUDE_VULKAN_RENDERER_H
#define INCLUDE_VULKAN_RENDERER_H

#include "defines.h"
#include "platform/platform.h"

#if PLATFORM_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#define VULKAN_DLL "vulkan-1.dll"

#define NUM_SWAPCHAIN_IMAGES 2 // TODO: Make this editable on runtime

namespace HY3D
{
	namespace Vulkan
	{
		global_var struct {
			dynamic_library library;

			VkInstance instance;
			VkDevice device;

			VkPhysicalDevice gpu;
			VkPhysicalDeviceProperties gpuProperties;
			VkPhysicalDeviceMemoryProperties gpuMemoryProperties;

			VkSurfaceKHR surface;
			VkSurfaceFormatKHR surfaceFormat;

			VkSampleCountFlagBits msaaSamples;

			VkQueue graphicsQueue;
			VkQueue presentQueue;
			VkQueue transferQueue;
			u32 graphicsQueueFamilyIndex;
			u32 presentQueueFamilyIndex;
			u32 transferQueueFamilyIndex;

			bool canRender;
			VkExtent2D windowExtent;
			VkSwapchainKHR swapchain;
			VkRenderPass renderPass;
			VkImage swapchainImages[NUM_SWAPCHAIN_IMAGES];
			VkImageView swapchainImageViews[NUM_SWAPCHAIN_IMAGES];
			VkFramebuffer framebuffers[NUM_SWAPCHAIN_IMAGES];
			u32 swapchainImageCount;
#if _DEBUG
			VkDebugUtilsMessengerEXT debugMessenger;
#endif
		} context;

	} // namespace Vulkan

} // namespace HY3D


#if _DEBUG
#define VkSuccess(call) (call == VK_SUCCESS)
#define VkSuccessOrReturnFalse(call) \
	if (!VkSuccess(call)) return false;

#define VkGoodHandle(handle) (handle != VK_NULL_HANDLE)
#define VkGoodHandleOrReturnFalse(handle) \
	if (!VkGoodHandle(handle)) return false;
#else
#define VkSuccess(call) call
#endif

#endif // INCLUDE_VULKAN_RENDERER_H