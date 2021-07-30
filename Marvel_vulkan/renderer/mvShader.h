#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace Marvel {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphicsContext;

	//---------------------------------------------------------------------
	// mvShader
	//---------------------------------------------------------------------
	class mvShader
	{

	public:

		mvShader(mvGraphicsContext& graphics, const std::string& file);

		VkShaderModule getShaderModule() const;

	private:

		VkShaderModule _shaderModule = VK_NULL_HANDLE;

	};

}