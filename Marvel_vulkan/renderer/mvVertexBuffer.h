#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "mvVertexLayout.h"

namespace Marvel {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphicsContext;
	class mvCommandBuffer;

	//---------------------------------------------------------------------
	// mvVertexBuffer
	//---------------------------------------------------------------------
	class mvVertexBuffer
	{

	public:

		mvVertexBuffer(mvGraphicsContext& graphics, const std::vector<float>& vbuf);

		void finish(mvGraphicsContext& graphics);
		void bind(mvCommandBuffer& commandBuffer);

	private:

		std::vector<float>   _vertices;
		VkBuffer             _vertexBuffer;
		VkDeviceMemory       _vertexBufferMemory;

	};

}