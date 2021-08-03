#include "mvVertexBuffer.h"
#include <stdexcept>
#include "mvGraphics.h"

namespace Marvel {

    mvVertexBuffer::mvVertexBuffer(mvGraphics& graphics, const std::vector<float>& vbuf)
        :
        _vertices(vbuf)
	{

        VkDeviceSize bufferSize = sizeof(float) * _vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        graphics.createBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(graphics.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, _vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(graphics.getDevice(), stagingBufferMemory);

        graphics.createBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            _vertexBuffer, _vertexBufferMemory);

        graphics.copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

        vkDestroyBuffer(graphics.getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(graphics.getDevice(), stagingBufferMemory, nullptr);
	}

    void mvVertexBuffer::bind(mvGraphics& graphics)
    {
        VkBuffer vertexBuffers[] = { _vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(graphics.getCurrentCommandBuffer(), 0, 1, vertexBuffers, offsets);
    }
}