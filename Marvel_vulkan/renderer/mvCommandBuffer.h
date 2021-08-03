#pragma once

#include <vulkan/vulkan.h>

namespace Marvel {

	//-----------------------------------------------------------------------------
	//forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	class mvCommandBuffer
	{

		friend class mvGraphics;

	public:

		void beginRecording(mvGraphics& graphics);
		void endRecording();
		void draw(uint32_t vertexCount);

		const VkCommandBuffer* getCommandBuffer();

	private:

		VkCommandBuffer _commandBuffer;
	};

}