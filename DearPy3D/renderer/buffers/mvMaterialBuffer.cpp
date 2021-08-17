#include "mvMaterialBuffer.h"
#include <stdexcept>
#include "mvGraphics.h"
#include "mvDrawable.h"

namespace DearPy3D {

	mvMaterialBuffer::mvMaterialBuffer()
	{
		_buffer.resize(3);
		_bufferMemory.resize(3);
		for (int i = 0; i < 3; i++)
		{
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = 2 * mvGraphics::GetContext().getPhysicalDevice().getRequiredUniformBufferSize(sizeof(mvMaterialData));
			bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(mvGraphics::GetContext().getLogicalDevice(), &bufferInfo, nullptr, &_buffer[i]) != VK_SUCCESS)
				throw std::runtime_error("failed to create buffer!");

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(mvGraphics::GetContext().getLogicalDevice(), _buffer[i], &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = mvGraphics::GetContext().getPhysicalDevice().findMemoryType(
				memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			if (vkAllocateMemory(mvGraphics::GetContext().getLogicalDevice(), &allocInfo, nullptr, &_bufferMemory[i]) != VK_SUCCESS)
				throw std::runtime_error("failed to allocate buffer memory!");

			vkBindBufferMemory(mvGraphics::GetContext().getLogicalDevice(), _buffer[i], _bufferMemory[i], 0);
		}
	}

	void mvMaterialBuffer::bind(mvMaterialData data, uint32_t index)
	{
		size_t offset_size = mvGraphics::GetContext().getPhysicalDevice().getRequiredUniformBufferSize(sizeof(mvMaterialData));
		auto cindex = mvGraphics::GetContext().getSwapChain().getCurrentImageIndex();
		void* udata;
		vkMapMemory(mvGraphics::GetContext().getLogicalDevice(), _bufferMemory[cindex], offset_size *index, offset_size, 0, &udata);
		memcpy(udata, &data, sizeof(mvMaterialData));
		vkUnmapMemory(mvGraphics::GetContext().getLogicalDevice(), _bufferMemory[cindex]);
	}

	void mvMaterialBuffer::cleanup()
	{
		for (int i = 0; i < 3; i++)
		{
			vkDestroyBuffer(mvGraphics::GetContext().getLogicalDevice(), _buffer[i], nullptr);
			vkFreeMemory(mvGraphics::GetContext().getLogicalDevice(), _bufferMemory[i], nullptr);
		}
	}
}