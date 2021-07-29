#pragma once

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Marvel {

	class mvDevice;

	class mvShader
	{

	public:

		mvShader(mvDevice& device, const std::string& file);
		~mvShader();

		VkShaderModule getShaderModule() const;

	private:

		VkShaderModule _shaderModule = VK_NULL_HANDLE;
		VkDevice       _device = VK_NULL_HANDLE;

	};

}