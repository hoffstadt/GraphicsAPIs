#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>


namespace Marvel {

	class mvGraphics;

	class mvVertexBuffer
	{

	public:

		mvVertexBuffer(mvGraphics& graphics, const std::vector<float>& vbuf);
		~mvVertexBuffer();

		void bind(VkCommandBuffer commandBuffer);

		VkVertexInputBindingDescription getBindingDescription();
		std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();

	private:

		std::vector<float> _vertices;

		VkDevice _device;
		VkBuffer _vertexBuffer;
		VkMemoryRequirements _memRequirements;
		VkDeviceMemory _vertexBufferMemory;

	};

}