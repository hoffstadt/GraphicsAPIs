#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace DearPy3D {

	class mvGraphics;
	class mvPhysicalDevice;

	class mvLogicalDevice
	{

	public:

		void init(mvPhysicalDevice& physicalDevice);
		void createCommandPool(mvGraphics& graphics, mvPhysicalDevice& physicalDevice);

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