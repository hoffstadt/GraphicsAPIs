#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "mvVertexLayout.h"


namespace Marvel {

	class mvGraphicsContext;

	class mvVertexBuffer
	{

	public:

		mvVertexBuffer(mvGraphicsContext& graphics, const mvVertexLayout& layout, const std::vector<float>& vbuf);

		void finish(mvGraphicsContext& graphics);
		void bind(VkCommandBuffer commandBuffer);
		const mvVertexLayout& GetLayout() const;

	private:

		std::vector<float>   _vertices;
		mvVertexLayout       _layout;
		VkBuffer             _vertexBuffer;
		VkMemoryRequirements _memRequirements;
		VkDeviceMemory       _vertexBufferMemory;

	};

}