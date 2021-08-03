#pragma once

#include <vulkan/vulkan.h>

namespace Marvel {

	class mvGraphics;

	class mvCommandBuffer
	{

		friend class mvCommandPool;

	public:

		mvCommandBuffer(uint32_t frameBufferTarget);

		void beginRecording(mvGraphics& graphics);
		void endRecording();
		void draw(uint32_t vertexCount);

		const VkCommandBuffer* getCommandBuffer();

	private:

		VkCommandBuffer _commandBuffer;
		uint32_t        _frameBufferTarget = 0u;
	};

}