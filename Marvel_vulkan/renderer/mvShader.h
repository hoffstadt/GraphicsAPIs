#pragma once

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Marvel {

	class mvGraphicsContext;

	class mvShader
	{

	public:

		mvShader(mvGraphicsContext& graphics, const std::string& file);

		void finish(mvGraphicsContext& graphics);

		VkShaderModule getShaderModule() const;

	private:

		VkShaderModule _shaderModule = VK_NULL_HANDLE;

	};

}