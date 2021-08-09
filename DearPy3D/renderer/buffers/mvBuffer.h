#pragma once

#include <vector>
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <mvGraphics.h>

namespace DearPy3D {
	
	//---------------------------------------------------------------------
	// mvBuffer
	//---------------------------------------------------------------------
	template <typename T>
	class mvBuffer
	{

	public:

		mvBuffer(mvGraphics& graphics, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
		{

			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = sizeof(T);
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			if (vkCreateBuffer(graphics.getLogicalDevice(), &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
				throw std::runtime_error("failed to create buffer!");

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(graphics.getLogicalDevice(), _buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = graphics.findMemoryType(memRequirements.memoryTypeBits, properties);

			if (vkAllocateMemory(graphics.getLogicalDevice(), &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS)
				throw std::runtime_error("failed to allocate buffer memory!");

			vkBindBufferMemory(graphics.getLogicalDevice(), _buffer, _bufferMemory, 0);

		}

		void update(mvGraphics& graphics, const T& data)
		{
			void* udata;
			vkMapMemory(graphics.getLogicalDevice(), _bufferMemory, 0, sizeof(T), 0, &udata);
			memcpy(udata, &data, sizeof(T));
			vkUnmapMemory(graphics.getLogicalDevice(), _bufferMemory);
		}

		void cleanup(mvGraphics& graphics)
		{
			vkDestroyBuffer(graphics.getLogicalDevice(), _buffer, nullptr);
			vkFreeMemory(graphics.getLogicalDevice(), _bufferMemory, nullptr);
		}

		VkBuffer getBuffer() { return _buffer; }

	private:

		VkBuffer       _buffer;
		VkDeviceMemory _bufferMemory;

	};

}