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

#define NUM_RESOURCES 4 // TODO: Make this editable on runtime
#define NUM_SWAPCHAIN_IMAGES 2 // TODO: Make this editable on runtime

namespace HY3D
{
	namespace Vulkan
	{
		enum VULKAN_FLAGS
		{
			INVALID_FLAG = 0,
			INITIALIZED  = 1,
			CANRENDER    = 2,
		};

		struct cmd_resources
		{
			VkCommandBuffer cmdBuffer;
			VkSemaphore imgAvailableSem;
			VkSemaphore frameReadySem;
			VkFence fence;
		};

		global_var struct vulkan_context{
			dynamic_library library;

			VkInstance instance;
			VkDevice device;

			VkPhysicalDevice gpu;
			VkPhysicalDeviceProperties gpuProperties;
			VkPhysicalDeviceMemoryProperties gpuMemoryProperties;

			VkSurfaceKHR surface;
			VkSurfaceFormatKHR surfaceFormat;

			VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT; // TODO: 

			VkQueue graphicsQueue;
			VkQueue presentQueue;
			VkQueue transferQueue;
			u32 graphicsQueueFamilyIndex;
			u32 presentQueueFamilyIndex;
			u32 transferQueueFamilyIndex;

			VkExtent2D windowExtent;
			VkSwapchainKHR swapchain;
			VkRenderPass renderPass;
			VkImage swapchainImages[NUM_SWAPCHAIN_IMAGES];
			VkImageView swapchainImageViews[NUM_SWAPCHAIN_IMAGES];
			VkFramebuffer framebuffers[NUM_SWAPCHAIN_IMAGES];
			u32 swapchainImageCount;

			VkCommandPool cmdBufferPool;
			cmd_resources cmdResources[NUM_RESOURCES];
			u32 currentCmdResource = 0;

			VkPipelineLayout pipelineLayout;
			VkPipeline pipeline;
			
			unsigned int flags;
#if _DEBUG
			VkDebugUtilsMessengerEXT debugMessenger;
#endif
		} *context;
	} // namespace Vulkan
} // namespace HY3D


#define VkSuccess(call) (call == VK_SUCCESS)
#define VkSuccessOrReturnFalse(call) \
	if (!VkSuccess(call)) return false

#define VkGoodHandle(handle) (handle != VK_NULL_HANDLE)
#define VkGoodHandleOrReturnFalse(handle) \
	if (!VkGoodHandle(handle)) return false
	
#endif // INCLUDE_VULKAN_RENDERER_H