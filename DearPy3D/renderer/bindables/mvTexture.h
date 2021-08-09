#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace DearPy3D {

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

		mvTexture(const std::string& file);

		void cleanup();

		VkImageView getImageView() { return _textureImageView; }

	private:

		VkImage        _textureImage;
		VkDeviceMemory _textureImageMemory;
		VkImageView    _textureImageView;
	};

}