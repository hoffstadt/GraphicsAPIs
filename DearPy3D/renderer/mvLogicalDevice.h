#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace DearPy3D {

	class mvGraphics;
	class mvPhysicalDevice;

	class mvLogicalDevice
	{

	public:

		// setup
		void init(mvPhysicalDevice& physicalDevice);
		void createCommandPool(mvGraphics& graphics, mvPhysicalDevice& physicalDevice);

		// resource utilities
		void        createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void        copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void        transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void        copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void        createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

		VkCommandBuffer beginSingleTimeCommands();
		void            endSingleTimeCommands(VkCommandBuffer commandBuffer);

		uint32_t getGraphicsQueueFamily() const { return _graphicsQueueFamily; }
		VkQueue getGraphicsQueue() { return _graphicsQueue; }
		VkQueue getPresentQueue() { return _presentQueue; }
		std::vector<VkCommandBuffer>& getCommandBuffers() { return _commandBuffers; }

		// temporary for render pass
		VkCommandPool getCommandPool() { return _commandPool; }

		operator VkDevice() { return _device; }

	private:

		mvPhysicalDevice*              _physicalDevice = nullptr;
		bool                           _enableValidationLayers = true;
		VkDevice                       _device = VK_NULL_HANDLE;
		VkQueue                        _graphicsQueue;
		VkQueue                        _presentQueue;
		uint32_t                       _graphicsQueueFamily = 0;
		VkCommandPool                  _commandPool;
		std::vector<VkCommandBuffer>   _commandBuffers;
		const std::vector<const char*> _validationLayers = { "VK_LAYER_KHRONOS_validation" };

	};

}