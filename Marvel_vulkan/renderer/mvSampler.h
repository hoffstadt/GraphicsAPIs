#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace Marvel {

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
		VkSampler getSampler() { return _textureSampler; }

	private:

		VkSampler _textureSampler;
	};

}