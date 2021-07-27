#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "mvVertexLayout.h"


namespace Marvel {

	class mvGraphics;

	class mvVertexBuffer
	{

	public:

		mvVertexBuffer(mvGraphics& graphics, const mvVertexLayout& layout, const std::vector<float>& vbuf);
		~mvVertexBuffer();

		void bind(VkCommandBuffer commandBuffer);
		const mvVertexLayout& GetLayout() const;

	private:

		std::vector<float>   _vertices;
		mvVertexLayout       _layout;
		VkDevice             _device;
		VkBuffer             _vertexBuffer;
		VkMemoryRequirements _memRequirements;
		VkDeviceMemory       _vertexBufferMemory;

	};

}