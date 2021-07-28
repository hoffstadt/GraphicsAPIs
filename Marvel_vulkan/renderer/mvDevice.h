#pragma once

#include <vector>
#include <array>
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Marvel {

	class mvDevice
	{

		friend class mvGraphics;

		struct QueueFamilyIndices 
		{

			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete()
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

	public:

		~mvDevice();

		void init(VkInstance instance, GLFWwindow* window, VkQueue& graphicsQueue, VkQueue& presentQueue);
		void createSwapChain(GLFWwindow* window, VkSwapchainKHR& swapchain, VkFormat& format, std::vector<VkImage>& images, VkExtent2D& extent);
		void createCommandPool(VkCommandPool& pool);

	private:

		void createSurface(VkInstance instance, GLFWwindow* window);
		void pickPhysicalDevice(VkInstance instance);
		void createLogicalDevice(VkQueue& graphicsQueue, VkQueue& presentQueue);


		// helpers
		bool                    checkValidationLayerSupport();
		bool                    isDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices      findQueueFamilies(VkPhysicalDevice device);
		bool                    checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	private:

		bool                           _enableValidationLayers = true;
		VkPhysicalDevice               _physicalDevice = VK_NULL_HANDLE;
		VkDevice                       _device = VK_NULL_HANDLE;
		VkSurfaceKHR                   _surface = VK_NULL_HANDLE;
		const std::vector<const char*> _validationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	};

}