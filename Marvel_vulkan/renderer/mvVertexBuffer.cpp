#include "mvVertexBuffer.h"
#include <stdexcept>
#include "mvGraphics.h"

namespace Marvel {

    mvVertexBuffer::mvVertexBuffer(mvGraphics& graphics, const mvVertexLayout& layout, const std::vector<float>& vbuf)
	{
        _device = graphics.getDevice();
        _vertices = vbuf;
        _layout = layout;

        VkDeviceSize bufferSize = sizeof(float) * _vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        graphics.createBuffer(bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, _vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(_device, stagingBufferMemory);

        graphics.createBuffer(bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            _vertexBuffer, _vertexBufferMemory);

        graphics.copyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

        vkDestroyBuffer(_device, stagingBuffer, nullptr);
        vkFreeMemory(_device, stagingBufferMemory, nullptr);
	}

    mvVertexBuffer::~mvVertexBuffer()
    {
        vkDestroyBuffer(_device, _vertexBuffer, nullptr);
        vkFreeMemory(_device, _vertexBufferMemory, nullptr);
    }

    const mvVertexLayout& mvVertexBuffer::GetLayout() const
    {
        return _layout;
    }

    void mvVertexBuffer::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer vertexBuffers[] = { _vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    }
}