#include "mvVertexBuffer.h"
#include <stdexcept>
#include "mvGraphics.h"

namespace DearPy3D {

    mvVertexBuffer::mvVertexBuffer(mvGraphics& graphics, const std::vector<float>& vbuf)
        :
        _vertices(vbuf)
	{
        VkDeviceSize bufferSize = sizeof(float) * _vertices.size();

        auto allocator = mvAllocator();

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkBuffer stagingBuffer;

        VmaAllocation stagingBufferAllocation = allocator.allocateBuffer(bufferInfo,
            VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

        float* data = allocator.mapMemory<float>(stagingBufferAllocation);
        memcpy(data, _vertices.data(), (size_t)bufferSize);
        allocator.unmapMemory(stagingBufferAllocation);

        VkBufferCreateInfo indexBufferInfo{};
        indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        indexBufferInfo.size = bufferSize;
        indexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

        _memoryAllocation = allocator.allocateBuffer(indexBufferInfo, VMA_MEMORY_USAGE_GPU_ONLY, _vertexBuffer);

        VkCommandBuffer copyCmd = graphics.getLogicalDevice().beginSingleTimeCommands();

        VkBufferCopy copyRegion = {};
        copyRegion.size = bufferSize;
        vkCmdCopyBuffer(
            copyCmd,
            stagingBuffer,
            _vertexBuffer,
            1,
            &copyRegion);

        graphics.getLogicalDevice().endSingleTimeCommands(copyCmd);

        allocator.destroyBuffer(stagingBuffer, stagingBufferAllocation);

        //VkDebugUtilsObjectNameInfoEXT info = {};
        //info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
        //info.pObjectName = "vertex buffer man";
        //vkSetDebugUtilsObjectNameEXT(graphics.getDevice(), &info);
	}

    void mvVertexBuffer::bind(mvGraphics& graphics)
    {
        VkBuffer vertexBuffers[] = { _vertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(graphics.getSwapChain().getCurrentCommandBuffer(graphics), 0, 1, vertexBuffers, offsets);
    }

    void mvVertexBuffer::cleanup(mvGraphics& graphics)
    {
        auto allocator = mvAllocator();
        vkDestroyBuffer(graphics.getLogicalDevice(), _vertexBuffer, nullptr);
        allocator.free(_memoryAllocation);
    }
}