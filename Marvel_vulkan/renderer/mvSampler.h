#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace Marvel {

	class mvGraphics;

	class mvSampler
	{

	public:

		mvSampler(mvGraphics& graphics);
		VkSampler getSampler() { return _textureSampler; }

	private:

		VkSampler _textureSampler;
	};

}