#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>


namespace Marvel {

	class mvGraphics;
	class mvCommandBuffer;

	class mvIndexBuffer
	{

	public:

		mvIndexBuffer(mvGraphics& graphics, const std::vector<uint16_t>& ibuf);

		void bind(mvCommandBuffer& commandBuffer);
		uint32_t getVertexCount();

	private:

		std::vector<uint16_t> _indices;

		VkBuffer _indexBuffer;
		VkDeviceMemory _indexBufferMemory;

	};

}