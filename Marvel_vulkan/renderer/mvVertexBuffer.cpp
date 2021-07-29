#include "mvVertexBuffer.h"
#include <stdexcept>
#include "mvDevice.h"
#include "mvGraphicsContext.h"

namespace Marvel {

    mvVertexBuffer::mvVertexBuffer(mvGraphicsContext& graphics, const mvVertexLayout& layout, const std::vector<float>& vbuf)
	{
        _vertices = vbuf;
        _layout = layout;

        VkDeviceSize bufferSize = sizeof(float) * _vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        graphics.getDevice().createBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(graphics.getDevice().getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, _vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(graphics.getDevice().getDevice(), stagingBufferMemory);

        graphics.getDevice().createBuffer(bufferSize,
            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
            _vertexBuffer, _vertexBufferMemory);

        graphics.getDevice().copyBuffer(graphics, stagingBuffer, _vertexBuffer, bufferSize);

        vkDestroyBuffer(graphics.getDevice().getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(graphics.getDevice().getDevice(), stagingBufferMemory, nullptr);
	}

    void mvVertexBuffer::finish(mvGraphicsContext& graphics)
    {
        vkDestroyBuffer(graphics.getDevice().getDevice(), _vertexBuffer, nullptr);
        vkFreeMemory(graphics.getDevice().getDevice(), _vertexBufferMemory, nullptr);
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