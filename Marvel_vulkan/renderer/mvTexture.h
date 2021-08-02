#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace Marvel {

	class mvGraphicsContext;

	class mvTexture
	{

	public:

		mvTexture(mvGraphicsContext& graphics, const std::string& file);
		VkImageView getImageView() { return _textureImageView; }

	private:

		VkImage        _textureImage;
		VkDeviceMemory _textureImageMemory;
		VkImageView    _textureImageView;
	};

}