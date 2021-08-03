#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace Marvel {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphics;

	//---------------------------------------------------------------------
	// mvShader
	//---------------------------------------------------------------------
	class mvShader
	{

	public:

		mvShader(mvGraphics& graphics, const std::string& file);

		VkShaderModule getShaderModule() const;

	private:

		VkShaderModule _shaderModule = VK_NULL_HANDLE;

	};

}