#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvTexture
	//-----------------------------------------------------------------------------
	class mvTexture
	{

	public:

		mvTexture(mvGraphics& graphics, const std::string& file);
		VkImageView getImageView() { return _textureImageView; }

	private:

		VkImage        _textureImage;
		VkDeviceMemory _textureImageMemory;
		VkImageView    _textureImageView;
	};

}