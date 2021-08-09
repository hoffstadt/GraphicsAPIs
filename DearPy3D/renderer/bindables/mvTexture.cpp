#include "mvTexture.h"
#include <stdexcept>
#include "mvGraphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace DearPy3D {

	mvTexture::mvTexture(mvGraphics& graphics, const std::string& file)
	{
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(file.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels)
            throw std::runtime_error("failed to load texture image!");

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        graphics.getLogicalDevice().createBuffer(imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(graphics.getLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(graphics.getLogicalDevice(), stagingBufferMemory);

        stbi_image_free(pixels);

        graphics.getLogicalDevice().createImage(texWidth, texHeight,
            VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            _textureImage, _textureImageMemory);

        graphics.getLogicalDevice().transitionImageLayout(_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        graphics.getLogicalDevice().copyBufferToImage(stagingBuffer, _textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        graphics.getLogicalDevice().transitionImageLayout(_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(graphics.getLogicalDevice(), stagingBuffer, nullptr);
        vkFreeMemory(graphics.getLogicalDevice(), stagingBufferMemory, nullptr);

        _textureImageView = graphics.getLogicalDevice().createImageView(_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
	}

    void mvTexture::cleanup(mvGraphics& graphics)
    {
        vkDestroyImageView(graphics.getLogicalDevice(), _textureImageView, nullptr);
        vkDestroyImage(graphics.getLogicalDevice(), _textureImage, nullptr);
        vkFreeMemory(graphics.getLogicalDevice(), _textureImageMemory, nullptr);
    }
}