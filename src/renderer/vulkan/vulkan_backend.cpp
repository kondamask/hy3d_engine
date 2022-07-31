#include "defines.h"
#include "core/logger.h"
#include "core/assert.h"
#include "core/utils.h"
#include "vulkan_renderer.h"

#include "vulkan_functions.cpp"

namespace HY3D
{
	namespace Vulkan
	{
#if _DEBUG
		static_func VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
			void* pUserData)
		{
			if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
			{
				LOG_WARN(callbackData->pMessage);
			}
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
			{
				LOG_INFO(callbackData->pMessage);
			}
			else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
			{
				LOG_INFO(callbackData->pMessage);
			}
			else
			{
				LOG_ERROR(callbackData->pMessage);
			}

			return VK_FALSE;
		}
#endif

		static_func bool CreateInstance(platform_state *platformState)
		{
			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = PlatformGetAppName(platformState);
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = appInfo.pApplicationName;
			appInfo.engineVersion = VK_API_VERSION_1_0;
			appInfo.apiVersion = VK_API_VERSION_1_2;

#if _DEBUG
			char* instanceLayers[] = {
				"VK_LAYER_KHRONOS_validation" };
#else
			char** instanceLayers = 0;
#endif

			if (instanceLayers)
			{
				u32 layerCount;
				VkSuccessOrReturnFalse(vkEnumerateInstanceLayerProperties(&layerCount, 0));

				VkLayerProperties availableLayers[32];
				ASSERT(layerCount <= ArrayCount(availableLayers));

				VkSuccessOrReturnFalse(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers));

				for (u32 i = 0; i < ArrayCount(instanceLayers); i++)
				{
					char* layerName = instanceLayers[i];
					bool found = false;
					for (VkLayerProperties& layerProperties : availableLayers)
					{
						if (strcmp(layerName, layerProperties.layerName) == 0)
						{
							found = true;
							break;
						}
					}
					if (!found)
					{
						ASSERT(found);
						return false;
					}
				}
			}

			char* extensions[] = {
		#if _DEBUG
				VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		#endif

				VK_KHR_SURFACE_EXTENSION_NAME,
		#if defined(VK_USE_PLATFORM_WIN32_KHR)
				VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		#elif defined(VK_USE_PLATFORM_XCB_KHR)
				VK_KHR_XCB_SURFACE_EXTENSION_NAME,
		#elif defined(VK_USE_PLATFORM_XLIB_KHR)
				VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
		#endif

				VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
			};

			u32 availableExtensionsCount;
			VkExtensionProperties availableInstanceExtensions[255] = {};
			VkSuccessOrReturnFalse(vkEnumerateInstanceExtensionProperties(0, &availableExtensionsCount, 0));

			ASSERT(availableExtensionsCount <= ArrayCount(availableInstanceExtensions));
			VkSuccessOrReturnFalse(vkEnumerateInstanceExtensionProperties(0, &availableExtensionsCount, availableInstanceExtensions));

			for (char* desiredInstanceExtension : extensions)
			{
				bool found = false;
				for (VkExtensionProperties& availableExtension : availableInstanceExtensions)
				{
					if (strcmp(desiredInstanceExtension, availableExtension.extensionName) == 0)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					ASSERT(found);
					return false;
				}
			}

			VkInstanceCreateInfo instanceInfo = {};
			instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceInfo.pApplicationInfo = &appInfo;
			instanceInfo.enabledExtensionCount = ArrayCount(extensions);
			instanceInfo.ppEnabledExtensionNames = extensions;
			if (instanceLayers)
			{
				instanceInfo.enabledLayerCount = ArrayCount(instanceLayers);
				instanceInfo.ppEnabledLayerNames = instanceLayers;
			}

#if _DEBUG
			VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
			debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugMessengerInfo.messageSeverity =
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
				// VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
				// VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
				;
			debugMessengerInfo.messageType =
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugMessengerInfo.pfnUserCallback = DebugCallback;

			instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;
#endif

			VkSuccessOrReturnFalse(vkCreateInstance(&instanceInfo, 0, &context.instance));

			if (!LoadInstanceFunctions())
				ASSERT(false);
#if _DEBUG
			VkSuccessOrReturnFalse(vkCreateDebugUtilsMessengerEXT(context.instance, &debugMessengerInfo, 0, &context.debugMessenger));
#endif
			return true;
		}

		static_func bool PickGPU()
		{
			u32 gpuCount = 0;
			VkPhysicalDevice gpuBuffer[16] = {};
			VkSuccessOrReturnFalse(vkEnumeratePhysicalDevices(context.instance, &gpuCount, 0));
			ASSERT(gpuCount > 0 && gpuCount <= ArrayCount(gpuBuffer));
			VkSuccessOrReturnFalse(vkEnumeratePhysicalDevices(context.instance, &gpuCount, gpuBuffer));
			context.gpu = gpuBuffer[0];

			// TODO: ACTUALY CHECK WHICH GPU IS BEST TO USE BY CHECKING THEIR QUEUES
			// For now it's ok since I only have 1 gpu.

			vkGetPhysicalDeviceProperties(context.gpu, &context.gpuProperties);
			vkGetPhysicalDeviceMemoryProperties(context.gpu, &context.memoryProperties);

			LOG_INFO("GPU: %s", context.gpuProperties.deviceName);

			return true;
		}

		static_func bool CreateSurface(platform_state* platformState)
		{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			VkWin32SurfaceCreateInfoKHR surfaceInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
			PlatformCreateVulkanSurface(platformState, &surfaceInfo);
			VkSuccessOrReturnFalse(vkCreateWin32SurfaceKHR(context.instance, &surfaceInfo, 0, &context.surface));

#elif defined(VK_USE_PLATFORM_XCB_KHR)
			VkXcbSurfaceCreateInfoKHR surfaceInfo = {};
			surfaceInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
			surfaceInfo.connection = 0; // we'll have these if we implement a linux window
			surfaceInfo.window = 0;		// we'll have these if we implement a linux window
			VkSuccessOrReturnFalse(vkCreateXcbSurfaceKHR(context.instance, &surfaceInfo, 0, &context.surface));

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
			VkXcbSurfaceCreateInfoKHR surfaceInfo = {};
			surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
			surfaceInfo.dpy = 0;	// we'll have these if we implement a mac(?) window
			surfaceInfo.window = 0; // we'll have these if we implement a mac(?) window
			VkSuccessOrReturnFalse(vkCreateXlibSurfaceKHR(context.instance, &surfaceInfo, 0, &context.surface));
#endif

			context.surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
			context.surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;

			u32 formatCount = 0;
			VkSurfaceFormatKHR availableFormats[16] = {};
			bool desiredSurfaceFormatSupported = false;
			VkSuccessOrReturnFalse(vkGetPhysicalDeviceSurfaceFormatsKHR(context.gpu, context.surface, &formatCount, 0));
			ASSERT(formatCount <= ArrayCount(availableFormats));

			for (u32 i = 0; i < formatCount; ++i)
				VkSuccessOrReturnFalse(vkGetPhysicalDeviceSurfaceFormatsKHR(context.gpu, context.surface, &formatCount, &availableFormats[i]));
			for (u32 i = 0; i < formatCount; ++i)
			{
				if (context.surfaceFormat.format == availableFormats[i].format &&
					context.surfaceFormat.colorSpace == availableFormats[i].colorSpace)
					desiredSurfaceFormatSupported = true;
			}
			ASSERT(desiredSurfaceFormatSupported);

			return desiredSurfaceFormatSupported;
		}

		static_func bool PickCommandQueues()
		{
			// TODO: Pick transfer queue from seperate family from that of graphics queue.
			bool result = true;

			context.graphicsQueueFamilyIndex = UINT32_MAX;
			context.presentQueueFamilyIndex = UINT32_MAX;
			context.transferQueueFamilyIndex = UINT32_MAX;
			{
				u32 queueFamilyCount;
				VkQueueFamilyProperties availableQueueFamilies[16] = {};
				VkBool32 supportsPresent[16] = {};
				vkGetPhysicalDeviceQueueFamilyProperties(context.gpu, &queueFamilyCount, 0);
				ASSERT(queueFamilyCount <= ArrayCount(availableQueueFamilies));
				vkGetPhysicalDeviceQueueFamilyProperties(context.gpu, &queueFamilyCount, availableQueueFamilies);

				for (u32 i = 0; i < queueFamilyCount; i++)
					VkSuccessOrReturnFalse(vkGetPhysicalDeviceSurfaceSupportKHR(context.gpu, i, context.surface, &supportsPresent[i]));

				for (u32 i = 0; i < queueFamilyCount; ++i)
				{
					// NOTE(heyyod): Find a graphics queue
					if ((availableQueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
					{
						if (context.graphicsQueueFamilyIndex == UINT32_MAX)
							context.graphicsQueueFamilyIndex = i;
						if (supportsPresent[i] == VK_TRUE)
						{
							context.graphicsQueueFamilyIndex = i;
							context.presentQueueFamilyIndex = i;
							break;
						}
					}
				}
				for (u32 i = 0; i < queueFamilyCount; ++i)
				{
					// NOTE(heyyod): Find a transfer queue
					if ((availableQueueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0)
					{
						if (context.transferQueueFamilyIndex == UINT32_MAX)
							context.transferQueueFamilyIndex = i;
					}
				}
				if (context.presentQueueFamilyIndex == UINT32_MAX) // didn't find a queue that supports both graphics and present
				{
					for (u32 i = 0; i < queueFamilyCount; ++i)
						if (supportsPresent[i] == VK_TRUE)
						{
							context.presentQueueFamilyIndex = i;
							break;
						}
				}

				ASSERT(context.graphicsQueueFamilyIndex != UINT32_MAX);
				if (context.graphicsQueueFamilyIndex == UINT32_MAX)
					result = false;

				ASSERT(!(result && context.presentQueueFamilyIndex == UINT32_MAX));
				if (result && context.presentQueueFamilyIndex == UINT32_MAX)
					result = false;
			}

			// TODO: make this work for seperate queues if needed
			ASSERT(!(result && context.graphicsQueueFamilyIndex != context.presentQueueFamilyIndex));
			if (result && context.graphicsQueueFamilyIndex != context.presentQueueFamilyIndex)
				return false;


			// TODO: Need to do some stuff if they are different like:
			// VkDeviceQueueCreateInfo queueInfo[2] = {};
			// float queuePriority = 1.0; // must be array of size queueCount
			//
			// queueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			// queueInfo[0].queueFamilyIndex = graphicsQueueFamilyIndex;
			// queueInfo[0].queueCount = 1;
			// queueInfo[0].pQueuePriorities = &queuePriority;
			//
			// queueInfo[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			// queueInfo[1].queueFamilyIndex = presentQueueFamilyIndex;
			// queueInfo[1].queueCount = 1;
			// queueInfo[1].pQueuePriorities = &queuePriority;
			return result;
		}

		static_func bool CreateDevice()
		{
			// Create a device
			VkDeviceQueueCreateInfo queueInfo = {};

			float queuePriority = 1.0; // must be array of size queueCount
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = context.graphicsQueueFamilyIndex;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &queuePriority;

			char* desiredDeviceExtensions[] = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
				VK_KHR_MAINTENANCE3_EXTENSION_NAME,
				VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME };

			u32 deviceExtensionsCount;
			VkExtensionProperties availableDeviceExtensions[255] = {};
			VkSuccessOrReturnFalse(vkEnumerateDeviceExtensionProperties(context.gpu, 0, &deviceExtensionsCount, 0));
			ASSERT(deviceExtensionsCount <= ArrayCount(availableDeviceExtensions));
			VkSuccessOrReturnFalse(vkEnumerateDeviceExtensionProperties(context.gpu, 0, &deviceExtensionsCount, availableDeviceExtensions));
			for (char* desiredDeviceExtension : desiredDeviceExtensions)
			{
				bool found = false;
				for (VkExtensionProperties& availableExtension : availableDeviceExtensions)
				{
					if (strcmp(desiredDeviceExtension, availableExtension.extensionName) == 0)
					{
						found = true;
						break;
					}
				}
				ASSERT(found);
			}

			// TODO(heyyod): Check if these features are supported;
			VkPhysicalDeviceFeatures desiredFeatures = {};
			desiredFeatures.samplerAnisotropy = VK_TRUE;
			desiredFeatures.fillModeNonSolid = VK_TRUE;
			desiredFeatures.wideLines = VK_TRUE;

			// TODO: I know these are supported by my gpu but I should check for them
			VkPhysicalDeviceDescriptorIndexingFeatures descriptorIndexingFeatures = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES };
			descriptorIndexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
			descriptorIndexingFeatures.runtimeDescriptorArray = VK_TRUE;
			descriptorIndexingFeatures.descriptorBindingVariableDescriptorCount = VK_TRUE;

			VkDeviceCreateInfo deviceInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
			deviceInfo.queueCreateInfoCount = 1; // NOTE: Only if graphicsQueueFamilyIndex == presentQueueFamilyIndex
			deviceInfo.pQueueCreateInfos = &queueInfo;
			deviceInfo.enabledExtensionCount = ArrayCount(desiredDeviceExtensions);
			deviceInfo.ppEnabledExtensionNames = desiredDeviceExtensions;
			deviceInfo.pEnabledFeatures = &desiredFeatures;

			deviceInfo.pNext = &descriptorIndexingFeatures;

			VkSuccessOrReturnFalse(vkCreateDevice(context.gpu, &deviceInfo, 0, &context.device));

			if (!LoadDeviceFunctions())
			{
				return false;
			}

			vkGetDeviceQueue(context.device, context.graphicsQueueFamilyIndex, 0, &context.graphicsQueue);
			vkGetDeviceQueue(context.device, context.presentQueueFamilyIndex, 0, &context.presentQueue);

			return true;
		}

	} // namespace Vulkan

} // namespace HY3D
