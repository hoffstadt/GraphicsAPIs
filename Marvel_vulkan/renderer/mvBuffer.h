#pragma once

#include <vector>
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

		mvBuffer(mvGraphicsContext& graphics, size_t count, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
		{

			_buffers.resize(count);
			_buffersMemory.resize(count);

			for (size_t i = 0; i < count, i++)
			{
				VkBufferCreateInfo bufferInfo{};
				bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferInfo.size = sizeof(T);
				bufferInfo.usage = usage;
				bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				if (vkCreateBuffer(graphics.getDevice().getDevice(), &bufferInfo, nullptr, &_buffers[i]) != VK_SUCCESS)
					throw std::runtime_error("failed to create buffer!");

				VkMemoryRequirements memRequirements;
				vkGetBufferMemoryRequirements(graphics.getDevice().getDevice(), _buffers[i], &memRequirements);

				VkMemoryAllocateInfo allocInfo{};
				allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				allocInfo.allocationSize = memRequirements.size;
				allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

				if (vkAllocateMemory(graphics.getDevice().getDevice(), &allocInfo, nullptr, &_buffersMemory[i]) != VK_SUCCESS)
					throw std::runtime_error("failed to allocate buffer memory!");

				vkBindBufferMemory(graphics.getDevice().getDevice(), _buffers[i], _buffersMemory[i], 0);
			}
		}

		void update(mvGraphicsContext& graphics, uint32_t index, const T& data)
		{
			void* udata;
			vkMapMemory(graphics.getDevice().getDevice(), _buffersMemory[index], 0, sizeof(T), 0, &udata);
			memcpy(udata, &data, sizeof(T));
			vkUnmapMemory(graphics.getDevice().getDevice(), _buffersMemory[index]);
		}

	private:

		std::vector<VkBuffer>       _buffers;
		std::vector<VkDeviceMemory> _buffersMemory;

	};

}