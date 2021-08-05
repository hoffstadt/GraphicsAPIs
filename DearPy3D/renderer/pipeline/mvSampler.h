#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace DearPy3D {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvSampler
	//-----------------------------------------------------------------------------
	class mvSampler
	{

	public:

		mvSampler(mvGraphics& graphics);
		
		void cleanup(mvGraphics& graphics);
		VkSampler getSampler() { return _textureSampler; }

	private:

		VkSampler _textureSampler;
	};

}