#include "mvIndexBuffer.h"
#include <stdexcept>
#include "mvGraphics.h"

namespace DearPy3D {

	mvIndexBuffer::mvIndexBuffer(mvGraphics& graphics, const std::vector<uint16_t>& ibuf)
	{
        _indices = ibuf;

        VkDeviceSize bufferSize = sizeof(_indices[0]) * _indices.size();

        auto allocator = mvAllocator();

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkBuffer stagingBuffer;

        VmaAllocation stagingBufferAllocation = allocator.allocateBuffer(bufferInfo,
            VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

        uint16_t* data = allocator.mapMemory<uint16_t>(stagingBufferAllocation);
        memcpy(data, _indices.data(), (size_t)bufferSize);
        allocator.unmapMemory(stagingBufferAllocation);

        VkBufferCreateInfo indexBufferInfo{};
        indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        indexBufferInfo.size = bufferSize;
        indexBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

        _memoryAllocation = allocator.allocateBuffer(indexBufferInfo, VMA_MEMORY_USAGE_GPU_ONLY, _indexBuffer);

        VkCommandBuffer copyCmd = graphics.getLogicalDevice().beginSingleTimeCommands();

        VkBufferCopy copyRegion = {};
        copyRegion.size = bufferSize;
        vkCmdCopyBuffer(
            copyCmd,
            stagingBuffer,
            _indexBuffer,
            1,
            &copyRegion);

        graphics.getLogicalDevice().endSingleTimeCommands(copyCmd);

        allocator.destroyBuffer(stagingBuffer, stagingBufferAllocation);
	}

    uint32_t mvIndexBuffer::getVertexCount()
    {
        return _indices.size();
    }

    void mvIndexBuffer::bind(mvGraphics& graphics)
    {
        vkCmdBindIndexBuffer(graphics.getCurrentCommandBuffer(), _indexBuffer, 0, VK_INDEX_TYPE_UINT16);
    }

    void mvIndexBuffer::cleanup(mvGraphics& graphics)
    {
        auto allocator = mvAllocator();
        vkDestroyBuffer(graphics.getLogicalDevice(), _indexBuffer, nullptr);
        allocator.free(_memoryAllocation);
    }

}