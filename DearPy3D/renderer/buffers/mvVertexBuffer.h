#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "mvVertexLayout.h"
#include "mvAllocator.h"

namespace DearPy3D {

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

		mvVertexBuffer(const std::vector<float>& vbuf);
		void cleanup();
		void bind();

	private:

		std::vector<float> _vertices;
		VkBuffer           _vertexBuffer;
		VmaAllocation      _memoryAllocation;

	};

}