#include "mvCommandBuffer.h"
#include <stdexcept>
#include "mvGraphicsContext.h"

namespace Marvel {

    mvCommandBuffer::mvCommandBuffer(uint32_t frameBufferTarget)
    {
        _frameBufferTarget = frameBufferTarget;
    }

	void mvCommandBuffer::beginRecording(mvGraphicsContext& graphics)
	{
 
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(_commandBuffer, &beginInfo) != VK_SUCCESS)
            throw std::runtime_error("failed to begin recording command buffer!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = graphics.getDevice().getRenderPass();
        renderPassInfo.framebuffer = graphics.getDevice().getFrameBuffer(_frameBufferTarget);
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = graphics.getDevice().getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = 2;
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}
	
	void mvCommandBuffer::endRecording()
	{
        vkCmdEndRenderPass(_commandBuffer);

        if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
            throw std::runtime_error("failed to record command buffer!");
	}

    void mvCommandBuffer::draw(uint32_t vertexCount)
    {
        vkCmdDrawIndexed(_commandBuffer, vertexCount, 1, 0, 0, 0);
    }

    const VkCommandBuffer* mvCommandBuffer::getCommandBuffer()
    {
        return &_commandBuffer;
    }

}