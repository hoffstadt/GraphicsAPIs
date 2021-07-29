#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>


namespace Marvel {

	class mvDevice;
	class mvGraphicsContext;

	class mvIndexBuffer
	{

	public:

		mvIndexBuffer(mvDevice& device, mvGraphicsContext& graphics, const std::vector<uint16_t>& ibuf);
		~mvIndexBuffer();

		void bind(VkCommandBuffer commandBuffer);
		uint32_t getVertexCount();

	private:

		std::vector<uint16_t> _indices;

		VkDevice _device;
		VkBuffer _indexBuffer;
		VkDeviceMemory _indexBufferMemory;

	};

}