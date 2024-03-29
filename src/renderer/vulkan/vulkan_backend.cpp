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

		static_func bool CreateInstance(platform_state* platformState)
		{
			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = PlatformGetAppName(platformState);
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = appInfo.pApplicationName;
			appInfo.engineVersion = VK_API_VERSION_1_0;
			appInfo.apiVersion = VK_API_VERSION_1_2;

			// TODO:  Fix this once there are instance layers needed for release
#if _DEBUG
			const char* instanceLayers[] = {
				"VK_LAYER_KHRONOS_validation"
			};

			u32 layerCount;
			VkSuccessOrReturnFalse(vkEnumerateInstanceLayerProperties(&layerCount, 0));

			VkLayerProperties availableLayers[32];
			ASSERT(layerCount <= ArrayCount(availableLayers));

			VkSuccessOrReturnFalse(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers));

			LOG_DEBUG("Required Instance Layers:");

			for (const char* desiredInstanceLayer : instanceLayers)
			{
				LOG_DEBUG("    %s", desiredInstanceLayer);
				bool found = false;
				for (VkLayerProperties& layerProperties : availableLayers)
				{
					if (strcmp(desiredInstanceLayer, layerProperties.layerName) == 0)
					{
						found = true;
						break;
					}
				}
				if (!found)
					LOG_ERROR("    %s - NOT SUPPORTED", desiredInstanceLayer);
				ASSERT(found);
			}
#endif

			const char* extensions[] = {
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

			LOG_DEBUG("Required Instance Extensions:");
			for (const char* desiredInstanceExtension : extensions)
			{
				LOG_DEBUG("    %s", desiredInstanceExtension);
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
					LOG_ERROR("    %s - NOT SUPPORTED", desiredInstanceExtension);
				ASSERT(found);
			}

			VkInstanceCreateInfo instanceInfo = {};
			instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceInfo.pApplicationInfo = &appInfo;
			instanceInfo.enabledExtensionCount = ArrayCount(extensions);
			instanceInfo.ppEnabledExtensionNames = extensions;

			// TODO:  Fix this once instance layers exist in release
#if _DEBUG
			instanceInfo.enabledLayerCount = ArrayCount(instanceLayers);
			instanceInfo.ppEnabledLayerNames = instanceLayers;
#endif

			VkSuccessOrReturnFalse(vkCreateInstance(&instanceInfo, 0, &context->instance));

			LOG_INFO(__FUNCTION__);

			if (!LoadInstanceFunctions())
				ASSERT(false);

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

			VkSuccessOrReturnFalse(vkCreateDebugUtilsMessengerEXT(context->instance, &debugMessengerInfo, 0, &context->debugMessenger));

			LOG_DEBUG("Created Vulkan Debug Messenger");
#endif

			return true;
		}

		static_func bool PickGPU()
		{
			u32 gpuCount = 0;
			VkPhysicalDevice gpuBuffer[16] = {};
			VkSuccessOrReturnFalse(vkEnumeratePhysicalDevices(context->instance, &gpuCount, 0));
			ASSERT(gpuCount > 0 && gpuCount <= ArrayCount(gpuBuffer));
			VkSuccessOrReturnFalse(vkEnumeratePhysicalDevices(context->instance, &gpuCount, gpuBuffer));

			i32 bestGPU = -1;
			i32 bestGPUScore = -1;
			VkPhysicalDeviceProperties bestGPUProperties = {};
			VkPhysicalDeviceMemoryProperties bestGPUMemoryProperties = {};

			LOG_INFO("Available GPUs:");
			for (u32 iGPU = 0; iGPU < gpuCount; iGPU++)
			{
				VkPhysicalDeviceProperties gpuProperties = {};
				VkPhysicalDeviceMemoryProperties gpuMemoryProperties = {};
				vkGetPhysicalDeviceProperties(gpuBuffer[iGPU], &gpuProperties);
				vkGetPhysicalDeviceMemoryProperties(gpuBuffer[iGPU], &gpuMemoryProperties);

				i32 curGPUScore = 0;
				switch (gpuProperties.deviceType)
				{
				case VK_PHYSICAL_DEVICE_TYPE_OTHER:
					LOG_INFO("    %s - UNKNOWN TYPE", gpuProperties.deviceName);
					curGPUScore += 1; break;
				case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
					LOG_INFO("    %s - VIRUTAL", gpuProperties.deviceName);
					curGPUScore += 2; break;
				case VK_PHYSICAL_DEVICE_TYPE_CPU:
					LOG_INFO("    %s - CPU", gpuProperties.deviceName);
					curGPUScore += 3; break;
				case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
					LOG_INFO("    %s - INTERGRATED", gpuProperties.deviceName);
					curGPUScore += 4; break;
				case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
					LOG_INFO("    %s - DISCRETE", gpuProperties.deviceName);
					curGPUScore += 5; break;
				}

				if (curGPUScore > bestGPUScore)
				{
					bestGPU = iGPU;
					bestGPUScore = curGPUScore;
					bestGPUProperties = gpuProperties;
					bestGPUMemoryProperties = gpuMemoryProperties;
				}
			}

			context->gpu = gpuBuffer[bestGPU];
			context->gpuProperties = bestGPUProperties;
			context->gpuMemoryProperties = bestGPUMemoryProperties;

			LOG_INFO("GPU Selected: %s", context->gpuProperties.deviceName);

			return true;
		}

		static_func bool CreateSurface(platform_state* platformState)
		{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
			VkWin32SurfaceCreateInfoKHR surfaceInfo = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
			PlatformCreateVulkanSurface(platformState, &surfaceInfo);
			VkSuccessOrReturnFalse(vkCreateWin32SurfaceKHR(context->instance, &surfaceInfo, 0, &context->surface));

#elif defined(VK_USE_PLATFORM_XCB_KHR)
			VkXcbSurfaceCreateInfoKHR surfaceInfo = {};
			surfaceInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
			surfaceInfo.connection = 0; // we'll have these if we implement a linux window
			surfaceInfo.window = 0;		// we'll have these if we implement a linux window
			VkSuccessOrReturnFalse(vkCreateXcbSurfaceKHR(context->instance, &surfaceInfo, 0, &context->surface));

#elif defined(VK_USE_PLATFORM_XLIB_KHR)
			VkXcbSurfaceCreateInfoKHR surfaceInfo = {};
			surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
			surfaceInfo.dpy = 0;	// we'll have these if we implement a mac(?) window
			surfaceInfo.window = 0; // we'll have these if we implement a mac(?) window
			VkSuccessOrReturnFalse(vkCreateXlibSurfaceKHR(context->instance, &surfaceInfo, 0, &context->surface));
#endif

			// TODO:  Check if supported?
			context->surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
			context->surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;

			u32 formatCount = 0;
			VkSurfaceFormatKHR availableFormats[16] = {};
			bool desiredSurfaceFormatSupported = false;
			VkSuccessOrReturnFalse(vkGetPhysicalDeviceSurfaceFormatsKHR(context->gpu, context->surface, &formatCount, 0));
			ASSERT(formatCount <= ArrayCount(availableFormats));

			for (u32 i = 0; i < formatCount; ++i)
				VkSuccessOrReturnFalse(vkGetPhysicalDeviceSurfaceFormatsKHR(context->gpu, context->surface, &formatCount, &availableFormats[i]));
			for (u32 i = 0; i < formatCount; ++i)
			{
				if (context->surfaceFormat.format == availableFormats[i].format &&
					context->surfaceFormat.colorSpace == availableFormats[i].colorSpace)
					desiredSurfaceFormatSupported = true;
			}
			ASSERT(desiredSurfaceFormatSupported);

			LOG_INFO(__FUNCTION__);

			return desiredSurfaceFormatSupported;
		}

		static_func bool PickCommandQueues()
		{
			context->graphicsQueueFamilyIndex = UINT32_MAX;
			context->presentQueueFamilyIndex = UINT32_MAX;
			context->transferQueueFamilyIndex = UINT32_MAX;
			{
				u32 queueFamilyCount;
				VkQueueFamilyProperties availableQueueFamilies[16] = {};
				VkBool32 supportsPresent[16] = {};
				vkGetPhysicalDeviceQueueFamilyProperties(context->gpu, &queueFamilyCount, 0);
				ASSERT(queueFamilyCount <= ArrayCount(availableQueueFamilies));
				vkGetPhysicalDeviceQueueFamilyProperties(context->gpu, &queueFamilyCount, availableQueueFamilies);

				for (u32 i = 0; i < queueFamilyCount; i++)
					VkSuccessOrReturnFalse(vkGetPhysicalDeviceSurfaceSupportKHR(context->gpu, i, context->surface, &supportsPresent[i]));

#if _DEBUG
				LOG_DEBUG("Available Queue Families:");
				for (u32 i = 0; i < queueFamilyCount; ++i)
				{
					LOG_DEBUG("    Family %d:", i);
					if (availableQueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
						LOG_DEBUG("      %d GRAPHICS Queues", availableQueueFamilies[i].queueCount);
					if (availableQueueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
						LOG_DEBUG("      %d COMPUTE Queues", availableQueueFamilies[i].queueCount);
					if (availableQueueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
						LOG_DEBUG("      %d TRANSFER Queues", availableQueueFamilies[i].queueCount);
					if (availableQueueFamilies[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
						LOG_DEBUG("      %d SPARSE_BINDING Queues", availableQueueFamilies[i].queueCount);
					if (availableQueueFamilies[i].queueFlags & VK_QUEUE_PROTECTED_BIT)
						LOG_DEBUG("      %d PROTECTED Queues", availableQueueFamilies[i].queueCount);						
				}
#endif
				// Pick Graphics and Present Queue Family
				for (u32 i = 0; i < queueFamilyCount; ++i)
				{
					if (availableQueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					{
						if (context->graphicsQueueFamilyIndex == UINT32_MAX)
							context->graphicsQueueFamilyIndex = i;
						if (supportsPresent[i] == VK_TRUE)
						{
							context->graphicsQueueFamilyIndex = i;
							context->presentQueueFamilyIndex = i;
							break;
						}
					}
				}

				ASSERT(context->graphicsQueueFamilyIndex != UINT32_MAX);
				ASSERT(context->graphicsQueueFamilyIndex == context->presentQueueFamilyIndex);
				LOG_INFO("Graphics Queue Family Selected: %d", context->graphicsQueueFamilyIndex);
				LOG_INFO("Present Queue Family Selected: %d", context->presentQueueFamilyIndex);

				// if (context->presentQueueFamilyIndex == UINT32_MAX) // didn't find a queue that supports both graphics and present
				// {
				// 	for (u32 i = 0; i < queueFamilyCount; ++i)
				// 		if (supportsPresent[i] == VK_TRUE)
				// 		{
				// 			context->presentQueueFamilyIndex = i;
				// 			break;
				// 		}
				// }

				// Pick Transfer Queue
				for (u32 i = 0; i < queueFamilyCount; ++i)
				{
					if (availableQueueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
					{
						if (context->transferQueueFamilyIndex == UINT32_MAX)
							context->transferQueueFamilyIndex = i;
					}
					
					if ((availableQueueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT) && 
						!(availableQueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
						context->graphicsQueueFamilyIndex != i)
					{
						context->transferQueueFamilyIndex = i;
						break;
					}
				}

				ASSERT(context->transferQueueFamilyIndex != UINT32_MAX);
				LOG_INFO("Transfer Queue Family Selected: %d", context->transferQueueFamilyIndex);
			}

			if (context->graphicsQueueFamilyIndex == UINT32_MAX ||
				context->presentQueueFamilyIndex == UINT32_MAX ||
				context->transferQueueFamilyIndex == UINT32_MAX)
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

			LOG_INFO(__FUNCTION__);

			return true;
		}

		static_func bool CreateDevice()
		{
			// Create a device
			VkDeviceQueueCreateInfo queueInfo = {};

			float queuePriority = 1.0; // must be array of size queueCount
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueFamilyIndex = context->graphicsQueueFamilyIndex;
			queueInfo.queueCount = 1;
			queueInfo.pQueuePriorities = &queuePriority;

			const char* desiredDeviceExtensions[] = {
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
				VK_KHR_MAINTENANCE3_EXTENSION_NAME,
				VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME };

			u32 deviceExtensionsCount;
			VkExtensionProperties availableDeviceExtensions[255] = {};
			VkSuccessOrReturnFalse(vkEnumerateDeviceExtensionProperties(context->gpu, 0, &deviceExtensionsCount, 0));
			ASSERT(deviceExtensionsCount <= ArrayCount(availableDeviceExtensions));
			VkSuccessOrReturnFalse(vkEnumerateDeviceExtensionProperties(context->gpu, 0, &deviceExtensionsCount, availableDeviceExtensions));

			LOG_DEBUG("Required Device Extensions:");
			for (const char* desiredDeviceExtension : desiredDeviceExtensions)
			{
				LOG_DEBUG("    %s", desiredDeviceExtension);
				bool found = false;
				for (VkExtensionProperties& availableExtension : availableDeviceExtensions)
				{
					if (strcmp(desiredDeviceExtension, availableExtension.extensionName) == 0)
					{
						found = true;
						break;
					}
				}
				if (!found)
					LOG_ERROR("    %s - NOT SUPPORTED", desiredDeviceExtension);
				ASSERT(found);
			}

			// TODOcontext->: Check if these features are supported;
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

			VkSuccessOrReturnFalse(vkCreateDevice(context->gpu, &deviceInfo, 0, &context->device));

			LOG_INFO(__FUNCTION__);

			if (!LoadDeviceFunctions())
			{
				return false;
			}

			vkGetDeviceQueue(context->device, context->graphicsQueueFamilyIndex, 0, &context->graphicsQueue);
			vkGetDeviceQueue(context->device, context->presentQueueFamilyIndex, 0, &context->presentQueue);

			return true;
		}

		static_func bool DestroySwapchainImages()
		{
			VkGoodHandleOrReturnFalse(context->device);
			vkDeviceWaitIdle(context->device);
			for (u32 i = 0; i < context->swapchainImageCount; i++)
			{
				if (VkGoodHandle(context->swapchainImageViews[i]))
					vkDestroyImageView(context->device, context->swapchainImageViews[i], 0);
			}
			return true;
		}

		static_func void DestroyRenderPass()
		{
			if (VkGoodHandle(context->device))
			{
				vkDeviceWaitIdle(context->device);
				if (VkGoodHandle(context->renderPass))
					vkDestroyRenderPass(context->device, context->renderPass, 0);
			}
		}

		static_func bool CreateRenderPass()
		{
			DestroyRenderPass();

			VkAttachmentDescription colorAttachment = {};
			colorAttachment.flags = 0;
			colorAttachment.format = context->surfaceFormat.format;
			colorAttachment.samples = context->msaaSamples;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			VkAttachmentReference colorAttachementRef = {};
			colorAttachementRef.attachment = 0;
			colorAttachementRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			// VkAttachmentDescription depthAttachment = {};
			// // depthAttachment.format = DEPTH_BUFFER_FORMAT;
			// depthAttachment.samples = context->msaaSamples;
			// depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			// depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			// depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			// depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			// depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			// depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			// VkAttachmentReference depthAttachmentRef = {};
			// depthAttachmentRef.attachment = 1;
			// depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			// NOTE: Specify the subpasses
			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

			// NOTE: The index of the color attachment in this array is directly
			// referenced from the fragment shader with the layout(location = 0) out vec4 outColor directive!
			subpass.colorAttachmentCount = 1; // ArrayCount(colorAttachementRef)
			subpass.pColorAttachments = &colorAttachementRef;
			// subpass.pDepthStencilAttachment = &depthAttachmentRef;

			// subpass.inputAttachmentCount = 0;
			// subpass.pInputAttachments = 0;
			// subpass.preserveAttachmentCount = 0;
			// subpass.pPreserveAttachments = 0;

			VkSubpassDependency dependency = {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;

			if (context->msaaSamples == VK_SAMPLE_COUNT_1_BIT)
			{
				colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				VkAttachmentDescription attachments[] = { 
					colorAttachment,
					// depthAttachment
				};
				renderPassInfo.attachmentCount = ArrayCount(attachments);
				renderPassInfo.pAttachments = attachments;
				VkSuccessOrReturnFalse(vkCreateRenderPass(context->device, &renderPassInfo, 0, &context->renderPass));
			}
			else
			{
				colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				// NOTE: context->: attachment to resolve the multisamplerd image into a presentable image
				VkAttachmentDescription colorAttachmentResolve = {};
				colorAttachmentResolve.format = context->surfaceFormat.format;
				colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
				colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				VkAttachmentReference colorAttachmentResolveRef = {};
				colorAttachmentResolveRef.attachment = 2;
				colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				subpass.pResolveAttachments = &colorAttachmentResolveRef;

				VkAttachmentDescription attachments[] = {
					colorAttachment,
					// depthAttachment,
					colorAttachmentResolve
				};
				renderPassInfo.attachmentCount = ArrayCount(attachments);
				renderPassInfo.pAttachments = attachments;
				VkSuccessOrReturnFalse(vkCreateRenderPass(context->device, &renderPassInfo, 0, &context->renderPass));
			}

			LOG_INFO(__FUNCTION__);
			return true;
		}

		static_func void DestroyFramebuffers()
		{
			if (VkGoodHandle(context->device))
			{
				vkDeviceWaitIdle(context->device);
				for (u32 i = 0; i < NUM_SWAPCHAIN_IMAGES; i++)
				{
					if (VkGoodHandle(context->framebuffers[i]))
					{
						vkDestroyFramebuffer(context->device, context->framebuffers[i], 0);
						// context->framebuffers[i] = VK_NULL_HANDLE;
					}
				}
			}
		}

		static_func bool CreateFramebuffers()
		{
			DestroyFramebuffers();

			local_var VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = context->renderPass;
			framebufferInfo.width = context->windowExtent.width;
			framebufferInfo.height = context->windowExtent.height;
			framebufferInfo.layers = 1;
			for (u32 i = 0; i < NUM_SWAPCHAIN_IMAGES; i++)
			{
				if (context->msaaSamples == VK_SAMPLE_COUNT_1_BIT)
				{
					VkImageView attachments[] = { 
						context->swapchainImageViews[i],
						// context->depthBuffer.view
					};
					framebufferInfo.pAttachments = attachments;
					framebufferInfo.attachmentCount = ArrayCount(attachments);
					VkSuccessOrReturnFalse(vkCreateFramebuffer(context->device, &framebufferInfo, 0, &context->framebuffers[i]));
				}
				else
				{
					VkImageView attachments[] = {
						// context->msaa.view,
						// context->depthBuffer.view,
						context->swapchainImageViews[i]
					};
					framebufferInfo.pAttachments = attachments;
					framebufferInfo.attachmentCount = ArrayCount(attachments);
					VkSuccessOrReturnFalse(vkCreateFramebuffer(context->device, &framebufferInfo, 0, &context->framebuffers[i]));
				}
			}

			LOG_INFO(__FUNCTION__);
			return true;
		}

		static_func bool CreateSwapchain()
		{
			bool result = true;

			VkGoodHandleOrReturnFalse(context->device);
			vkDeviceWaitIdle(context->device);

			BIT_CLEAR(context->flags, VULKAN_FLAGS::CANRENDER);

			// NOTE: Get Surface capabilities
			{
				VkSurfaceCapabilitiesKHR surfCapabilities = {};
				VkSuccessOrReturnFalse(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->gpu, context->surface, &surfCapabilities));

				context->windowExtent = {};
				if (surfCapabilities.currentExtent.width == UINT32_MAX)
				{
					context->windowExtent.width = 640;
					context->windowExtent.height = 480;

					if (context->windowExtent.width < surfCapabilities.minImageExtent.width)
						context->windowExtent.width = surfCapabilities.minImageExtent.width;
					else if (context->windowExtent.width > surfCapabilities.maxImageExtent.width)
						context->windowExtent.width = surfCapabilities.maxImageExtent.width;

					if (context->windowExtent.height < surfCapabilities.minImageExtent.height)
						context->windowExtent.height = surfCapabilities.minImageExtent.height;
					else if (context->windowExtent.height > surfCapabilities.maxImageExtent.height)
						context->windowExtent.height = surfCapabilities.maxImageExtent.height;
				}
				else
				{ // If the surface size is defined, the swap chain size must match
					context->windowExtent = surfCapabilities.currentExtent;
				}
				if ((context->windowExtent.width == 0) || (context->windowExtent.height == 0))
				{
					BIT_CLEAR(context->flags, VULKAN_FLAGS::CANRENDER);
					return true;
				}

				// NOTE: Determine the number of VkImage's to use in the swap chain.
				// Constant at 2 for now: Double buffering
				ASSERT(NUM_SWAPCHAIN_IMAGES >= surfCapabilities.minImageCount);
				ASSERT(NUM_SWAPCHAIN_IMAGES <= surfCapabilities.maxImageCount);
				context->swapchainImageCount = NUM_SWAPCHAIN_IMAGES;

				// NOTE: Determine the pre-transform
				VkSurfaceTransformFlagBitsKHR desiredPreTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; // do nothing
				if (!(surfCapabilities.supportedTransforms & desiredPreTransform))
					desiredPreTransform = surfCapabilities.currentTransform;

				// NOTE: Set the present mode
				VkPresentModeKHR desiredPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				// The FIFO present mode is guaranteed by the spec to be supported

				uint32_t presentModeCount;
				VkSuccessOrReturnFalse(vkGetPhysicalDeviceSurfacePresentModesKHR(context->gpu, context->surface, &presentModeCount, NULL));
				VkPresentModeKHR presentModes[16] = {};

				ASSERT(presentModeCount <= ArrayCount(presentModes));
				VkSuccessOrReturnFalse(vkGetPhysicalDeviceSurfacePresentModesKHR(context->gpu, context->surface, &presentModeCount, presentModes));
				bool desiredPresentModeSupported = false;
				for (u32 i = 0; i < presentModeCount; i++)
				{
					if (desiredPresentMode == presentModes[i])
						desiredPresentModeSupported = true;
				}
				if (!desiredPresentModeSupported)
				{
					desiredPresentMode = VK_PRESENT_MODE_FIFO_KHR; // VK_PRESENT_MODE_IMMEDIATE_KHR;
				}

				// NOTE: Find a supported composite alpha mode - one of these is guaranteed to be set
				VkCompositeAlphaFlagBitsKHR desiredCompositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
					VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
					VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
					VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
					VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
				};
				for (uint32_t i = 0; i < ArrayCount(compositeAlphaFlags); i++)
				{
					if (surfCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i])
					{
						desiredCompositeAlpha = compositeAlphaFlags[i];
						break;
					}
				}

				// NOTE: Set image usage flags
				VkImageUsageFlags desiredImageUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // must alwasy be supported
				if (surfCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
					desiredImageUsageFlags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
				else
				{
					LOG_ERROR("VK_IMAGE_USAGE_TRANSFER_DST_BIT not supported\n");
					return false;
				}

				// NOTE: Make the actual swapchain
				VkSwapchainCreateInfoKHR swapchainInfo = {};
				swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				swapchainInfo.surface = context->surface;
				swapchainInfo.imageExtent = context->windowExtent;
				swapchainInfo.imageFormat = context->surfaceFormat.format;
				swapchainInfo.imageColorSpace = context->surfaceFormat.colorSpace;
				swapchainInfo.imageArrayLayers = 1;
				swapchainInfo.imageUsage = desiredImageUsageFlags;
				swapchainInfo.minImageCount = context->swapchainImageCount;
				swapchainInfo.preTransform = desiredPreTransform;
				swapchainInfo.compositeAlpha = desiredCompositeAlpha;
				swapchainInfo.presentMode = desiredPresentMode;
				swapchainInfo.clipped = true;

				u32 queueFamilyIndices[2] = { context->graphicsQueueFamilyIndex, context->presentQueueFamilyIndex };
				if (context->graphicsQueueFamilyIndex != context->presentQueueFamilyIndex)
				{
					// If the graphics and present queues are from different queue families,
					// we either have to explicitly transfer ownership of images between
					// the queues, or we have to create the swapchain with imageSharingMode
					// as VK_SHARING_MODE_CONCURRENT
					swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
					swapchainInfo.queueFamilyIndexCount = 2;
					swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
				}
				else
				{
					swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				}

				VkSwapchainKHR oldSwapchain = context->swapchain;
				swapchainInfo.oldSwapchain = oldSwapchain;
				VkSuccessOrReturnFalse(vkCreateSwapchainKHR(context->device, &swapchainInfo, 0, &context->swapchain));

				if (VkGoodHandle(oldSwapchain))
					vkDestroySwapchainKHR(context->device, oldSwapchain, 0);
			}

			// NOTE: Create the Image views
			{
				if (!DestroySwapchainImages())
				{
					LOG_ERROR("Failed to clear swapchain images.");
					return false;
				}
				VkSuccessOrReturnFalse(vkGetSwapchainImagesKHR(context->device, context->swapchain, &context->swapchainImageCount, 0));
				ASSERT(context->swapchainImageCount == ArrayCount(context->swapchainImages));
				VkSuccessOrReturnFalse(vkGetSwapchainImagesKHR(context->device, context->swapchain, &context->swapchainImageCount, context->swapchainImages));

				VkImageViewCreateInfo imageViewInfo = {};
				imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				imageViewInfo.format = context->surfaceFormat.format;
				imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
				imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
				imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
				imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
				imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageViewInfo.subresourceRange.baseMipLevel = 0;
				imageViewInfo.subresourceRange.levelCount = 1;
				imageViewInfo.subresourceRange.baseArrayLayer = 0;
				imageViewInfo.subresourceRange.layerCount = 1;
				for (u32 i = 0; i < context->swapchainImageCount; i++)
				{
					imageViewInfo.image = context->swapchainImages[i];
					VkSuccessOrReturnFalse(vkCreateImageView(context->device, &imageViewInfo, 0, &context->swapchainImageViews[i]));
				}
			}

			// if (result)
			// 	result = VulkanCreateDepthBuffer();

			// if (result)
			// 	result = VulkanCreateMSAABuffer();

			if (result)
				result = CreateFramebuffers();

			if (result)
				BIT_SET(context->flags, VULKAN_FLAGS::CANRENDER);
			else
				BIT_CLEAR(context->flags, VULKAN_FLAGS::CANRENDER);

			LOG_INFO(__FUNCTION__);

			return true;
		}

		inline static_func bool CreateCommandResources()
		{
			VkCommandPoolCreateInfo cmdBufferPoolInfo = {};
			cmdBufferPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			cmdBufferPoolInfo.queueFamilyIndex = context->presentQueueFamilyIndex;
			cmdBufferPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
			VkSuccessOrReturnFalse(vkCreateCommandPool(context->device, &cmdBufferPoolInfo, 0, &context->cmdBufferPool));

			VkCommandBufferAllocateInfo cmdBufferAllocInfo = {};
			cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cmdBufferAllocInfo.commandPool = context->cmdBufferPool;
			cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			cmdBufferAllocInfo.commandBufferCount = 1;

			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			for (u32 i = 0; i < NUM_RESOURCES; i++)
			{
				VkSuccessOrReturnFalse(vkAllocateCommandBuffers(context->device, &cmdBufferAllocInfo, &context->cmdResources[i].cmdBuffer));
				VkSuccessOrReturnFalse(vkCreateSemaphore(context->device, &semaphoreInfo, 0, &context->cmdResources[i].imgAvailableSem));
				VkSuccessOrReturnFalse(vkCreateSemaphore(context->device, &semaphoreInfo, 0, &context->cmdResources[i].frameReadySem));
				VkSuccessOrReturnFalse(vkCreateFence(context->device, &fenceInfo, 0, &context->cmdResources[i].fence));
			}

			LOG_INFO(__FUNCTION__);
			return true;
		}

		static_func cmd_resources *GetNextAvailableCommandResource()
		{
			cmd_resources* result = &context->cmdResources[context->currentCmdResource];
			context->currentCmdResource = (context->currentCmdResource + 1) % NUM_RESOURCES;

			if (!VkSuccess(vkWaitForFences(context->device, 1, &result->fence, true, UINT64_MAX)))
				return 0;
			if (!VkSuccess(vkResetFences(context->device, 1, &result->fence)))
				return 0;

			return result;
		}

		static_func void DestroyPipeline()
		{
			vkDeviceWaitIdle(context->device);
			
			if (VkGoodHandle(context->pipeline))
			{
				vkDestroyPipeline(context->device, context->pipeline, 0);
				context->pipeline = 0;
			}
			if (VkGoodHandle(context->pipelineLayout))
			{
				vkDestroyPipelineLayout(context->device, context->pipelineLayout, 0);
				context->pipelineLayout = 0;
			}
		}

		static_func bool CreateGraphicsPipeline()
		{
			VkGraphicsPipelineCreateInfo graphicsPipelineInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };

			VkShaderModuleCreateInfo vertShaderModuleInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
			read_file_result vertShaderFile = PlatformReadFile("bin\\shaders\\triangle.vert.spv");
			vertShaderModuleInfo.pCode = (u32*)vertShaderFile.content;
			vertShaderModuleInfo.codeSize = vertShaderFile.size;
			VkShaderModule vertShaderModule;
			VkSuccessOrReturnFalse(vkCreateShaderModule(context->device, &vertShaderModuleInfo, 0, &vertShaderModule));

			VkShaderModuleCreateInfo fragShaderModuleInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
			read_file_result fragShaderFile = PlatformReadFile("bin\\shaders\\triangle.frag.spv");
			fragShaderModuleInfo.pCode = (u32*)fragShaderFile.content;
			fragShaderModuleInfo.codeSize = fragShaderFile.size;
			VkShaderModule fragShaderModule;
			VkSuccessOrReturnFalse(vkCreateShaderModule(context->device, &fragShaderModuleInfo, 0, &fragShaderModule));

			VkPipelineShaderStageCreateInfo stages[] = {
				{
					VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
					0,
					0,
					VK_SHADER_STAGE_VERTEX_BIT,
					vertShaderModule,
					"main",
				},
				{
					VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
					0,
					0,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					fragShaderModule,
					"main",
				}
			};
			graphicsPipelineInfo.pStages = stages;
			graphicsPipelineInfo.stageCount = ArrayCount(stages);

			VkPipelineVertexInputStateCreateInfo vertInput = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
			graphicsPipelineInfo.pVertexInputState = &vertInput;

			VkPipelineInputAssemblyStateCreateInfo inputAssembly = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			graphicsPipelineInfo.pInputAssemblyState = &inputAssembly;

			VkPipelineViewportStateCreateInfo viewportInfo = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
			viewportInfo.scissorCount = 1;
			viewportInfo.viewportCount = 1;
			graphicsPipelineInfo.pViewportState = &viewportInfo;

			VkPipelineRasterizationStateCreateInfo rasterizationState = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
			graphicsPipelineInfo.pRasterizationState = &rasterizationState;

			VkPipelineMultisampleStateCreateInfo multisampleState = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
			multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			graphicsPipelineInfo.pMultisampleState = &multisampleState;

			VkPipelineDepthStencilStateCreateInfo depthStenticState = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
			graphicsPipelineInfo.pDepthStencilState = &depthStenticState;

			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask =
				VK_COLOR_COMPONENT_R_BIT |
				VK_COLOR_COMPONENT_G_BIT |
				VK_COLOR_COMPONENT_B_BIT |
				VK_COLOR_COMPONENT_A_BIT;

			VkPipelineColorBlendStateCreateInfo colorBlendState = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
			colorBlendState.attachmentCount = 1;
			colorBlendState.pAttachments = &colorBlendAttachment;
			graphicsPipelineInfo.pColorBlendState = &colorBlendState;

			VkDynamicState dynamicStates[] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};

			VkPipelineDynamicStateCreateInfo dynamicStatesInfo = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
			dynamicStatesInfo.dynamicStateCount = ArrayCount(dynamicStates);
			dynamicStatesInfo.pDynamicStates = dynamicStates;

			graphicsPipelineInfo.pDynamicState = &dynamicStatesInfo;

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
			VkSuccessOrReturnFalse(vkCreatePipelineLayout(context->device, &pipelineLayoutInfo, 0, &context->pipelineLayout));
			graphicsPipelineInfo.layout = context->pipelineLayout;

			graphicsPipelineInfo.renderPass = context->renderPass;

			VkSuccessOrReturnFalse(vkCreateGraphicsPipelines(context->device, 0, 1, &graphicsPipelineInfo, 0, &context->pipeline));

			return true;
		}
	} // namespace Vulkan
} // namespace HY3D
