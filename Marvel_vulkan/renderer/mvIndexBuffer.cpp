#include "mvIndexBuffer.h"
#include <stdexcept>
#include "mvDevice.h"
#include "mvGraphicsContext.h"
#include "mvCommandBuffer.h"

namespace Marvel {

	mvIndexBuffer::mvIndexBuffer( mvGraphicsContext& graphics, const std::vector<uint16_t>& ibuf)
	{
        _indices = ibuf;

        VkDeviceSize bufferSize = sizeof(_indices[0]) * _indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        graphics.getDevice().createBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
            stagingBuffer, 
            stagingBufferMemory);

        void* data;
        vkMapMemory(graphics.getDevice().getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, _indices.data(), (size_t)bufferSize);
        vkUnmapMemory(graphics.getDevice().getDevice(), stagingBufferMemory);

        graphics.getDevice().createBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            _indexBuffer, _indexBufferMemory);

        graphics.getDevice().copyBuffer(graphics, stagingBuffer, _indexBuffer, bufferSize);

        vkDestroyBuffer(graphics.getDevice().getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(graphics.getDevice().getDevice(), stagingBufferMemory, nullptr);
	}

    uint32_t mvIndexBuffer::getVertexCount()
    {
        return _indices.size();
    }

    void mvIndexBuffer::bind(mvCommandBuffer& commandBuffer)
    {
        vkCmdBindIndexBuffer(*commandBuffer.getCommandBuffer(), _indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    }

}