#pragma once

#include <vulkan/vulkan.h>

namespace Marvel {

	class mvGraphics;
	class mvCommandBuffer;

	class mvCommandPool
	{

	public:

		mvCommandPool(mvGraphics& graphics);

		void allocateCommandBuffer(mvGraphics& graphics, mvCommandBuffer* commandBuffer);

	private:

		VkCommandPool _commandPool;
	};

}