#include "mvShader.h"
#include <fstream>
#include <stdexcept>
#include "mvDevice.h"

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

namespace Marvel {

	mvShader::mvShader(mvDevice& device, const std::string& file)
	{
        auto shaderCode = readFile(file);
        _shaderModule = device.createShaderModule(shaderCode);
        _device = device.getDevice();
	}

    mvShader::~mvShader()
    {
        vkDestroyShaderModule(_device, _shaderModule, nullptr);
    }

    VkShaderModule mvShader::getShaderModule() const
    {
        return _shaderModule;
    }

}