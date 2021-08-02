#pragma once

#include <vulkan/vulkan.h>

namespace Marvel {

	class mvGraphicsContext;
	class mvCommandBuffer;

	class mvCommandPool
	{

	public:

		mvCommandPool(mvGraphicsContext& graphics);

		void allocateCommandBuffer(mvGraphicsContext& graphics, mvCommandBuffer* commandBuffer);

	private:

		VkCommandPool _commandPool;
	};

}