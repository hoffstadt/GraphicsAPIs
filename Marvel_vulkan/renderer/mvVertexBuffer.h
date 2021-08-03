#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "mvVertexLayout.h"

namespace Marvel {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphics;

	//---------------------------------------------------------------------
	// mvVertexBuffer
	//---------------------------------------------------------------------
	class mvVertexBuffer
	{

	public:

		mvVertexBuffer(mvGraphics& graphics, const std::vector<float>& vbuf);

		void bind(mvGraphics& graphics);

	private:

		std::vector<float>   _vertices;
		VkBuffer             _vertexBuffer;
		VkDeviceMemory       _vertexBufferMemory;

	};

}