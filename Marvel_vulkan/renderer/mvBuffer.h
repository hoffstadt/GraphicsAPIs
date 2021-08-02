#pragma once

#include <vector>
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <mvGraphicsContext.h>

namespace Marvel {
	
	//---------------------------------------------------------------------
	// mvBuffer
	//---------------------------------------------------------------------
	template <typename T>
	class mvBuffer
	{

	public:

		mvBuffer(mvGraphicsContext& graphics, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
		{

			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = sizeof(T);
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(graphics.getDevice().getDevice(), &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
				throw std::runtime_error("failed to create buffer!");

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(graphics.getDevice().getDevice(), _buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = graphics.getDevice().findMemoryType(memRequirements.memoryTypeBits, properties);

			if (vkAllocateMemory(graphics.getDevice().getDevice(), &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS)
				throw std::runtime_error("failed to allocate buffer memory!");

			vkBindBufferMemory(graphics.getDevice().getDevice(), _buffer, _bufferMemory, 0);

		}

		void update(mvGraphicsContext& graphics, const T& data)
		{
			void* udata;
			vkMapMemory(graphics.getDevice().getDevice(), _bufferMemory, 0, sizeof(T), 0, &udata);
			memcpy(udata, &data, sizeof(T));
			vkUnmapMemory(graphics.getDevice().getDevice(), _bufferMemory);
		}

		VkBuffer getBuffer() { return _buffer; }

	private:

		VkBuffer       _buffer;
		VkDeviceMemory _bufferMemory;

	};

}