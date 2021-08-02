#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace Marvel {

	class mvGraphicsContext;

	class mvSampler
	{

	public:

		mvSampler(mvGraphicsContext& graphics);
		VkSampler getSampler() { return _textureSampler; }

	private:

		VkSampler _textureSampler;
	};

}