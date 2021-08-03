#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>


namespace DearPy3D {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvIndexBuffer
	//-----------------------------------------------------------------------------
	class mvIndexBuffer
	{

	public:

		mvIndexBuffer(mvGraphics& graphics, const std::vector<uint16_t>& ibuf);

		void bind(mvGraphics& graphics);
		uint32_t getVertexCount();

	private:

		std::vector<uint16_t> _indices;

		VkBuffer _indexBuffer;
		VkDeviceMemory _indexBufferMemory;

	};

}