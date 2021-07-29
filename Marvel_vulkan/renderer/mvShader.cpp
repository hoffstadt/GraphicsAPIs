#include "mvShader.h"
#include <fstream>
#include <stdexcept>
#include "mvGraphicsContext.h"

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

	mvShader::mvShader(mvGraphicsContext& graphics, const std::string& file)
	{
        auto shaderCode = readFile(file);
        _shaderModule = graphics.getDevice().createShaderModule(shaderCode);
	}

    VkShaderModule mvShader::getShaderModule() const
    {
        return _shaderModule;
    }

}