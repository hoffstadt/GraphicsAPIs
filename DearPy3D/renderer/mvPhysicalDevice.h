#pragma once

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

namespace DearPy3D {

	class mvGraphics;

	class mvPhysicalDevice
	{

	public:

		struct QueueFamilyIndices 
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;
		};

	public:

		void init();

		const std::vector<const char*>& getExtensions() const { return _deviceExtensions; }
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		std::uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		VkPhysicalDeviceProperties getProperties() { return _properties; }

		operator VkPhysicalDevice() { return _physicalDevice; }

	private:

		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	private:

		VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
		const std::vector<const char*> _deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkPhysicalDeviceProperties _properties;

	};

}