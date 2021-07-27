#include "mvIndexBuffer.h"
#include <stdexcept>
#include "mvGraphics.h"

namespace Marvel {

	mvIndexBuffer::mvIndexBuffer(mvGraphics& graphics, const std::vector<uint16_t>& ibuf)
	{
        _device = graphics.getDevice();
        _indices = ibuf;

        VkDeviceSize bufferSize = sizeof(_indices[0]) * _indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        graphics.createBuffer(bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            stagingBuffer, 
            stagingBufferMemory);

        void* data;
        vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, _indices.data(), (size_t)bufferSize);
        vkUnmapMemory(_device, stagingBufferMemory);

        graphics.createBuffer(bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            _indexBuffer, _indexBufferMemory);

        graphics.copyBuffer(stagingBuffer, _indexBuffer, bufferSize);

        vkDestroyBuffer(_device, stagingBuffer, nullptr);
        vkFreeMemory(_device, stagingBufferMemory, nullptr);
	}

    mvIndexBuffer::~mvIndexBuffer()
    {
        vkDestroyBuffer(_device, _indexBuffer, nullptr);
        vkFreeMemory(_device, _indexBufferMemory, nullptr);
    }

    uint32_t mvIndexBuffer::getVertexCount()
    {
        return _indices.size();
    }

    void mvIndexBuffer::bind(VkCommandBuffer commandBuffer)
    {
        vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    }

}