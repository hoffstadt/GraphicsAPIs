#include "mvCommandPool.h"
#include <stdexcept>
#include "mvGraphicsContext.h"
#include "mvCommandBuffer.h"

namespace Marvel {

	mvCommandPool::mvCommandPool(mvGraphicsContext& graphics)
	{
        mvDevice::QueueFamilyIndices queueFamilyIndices = graphics.getDevice().findQueueFamilies(graphics.getDevice().getPhysicalDevice());

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(graphics.getDevice().getDevice(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS)
            throw std::runtime_error("failed to create command pool!");
	}

    void mvCommandPool::allocateCommandBuffer(mvGraphicsContext& graphics, mvCommandBuffer* commandBuffer)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = _commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)1;

        if (vkAllocateCommandBuffers(graphics.getDevice().getDevice(), &allocInfo, &commandBuffer->_commandBuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate command buffers!");
    }
}