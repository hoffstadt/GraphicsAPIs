#include "mvShader.h"
#include <fstream>
#include <stdexcept>
#include "mvGraphics.h"

static std::vector<char> readFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("failed to open file!");

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

namespace DearPy3D {

	mvShader::mvShader(mvGraphics& graphics, const std::string& file)
	{
        auto shaderCode = readFile(file);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(graphics.getLogicalDevice(), &createInfo, nullptr, &_shaderModule) != VK_SUCCESS)
            throw std::runtime_error("failed to create shader module!");
	}

    VkShaderModule mvShader::getShaderModule() const
    {
        return _shaderModule;
    }

}