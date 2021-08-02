#include "mvTexture.h"
#include <stdexcept>
#include "mvGraphicsContext.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Marvel{

	mvTexture::mvTexture(mvGraphicsContext& graphics, const std::string& file)
	{
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(file.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels)
            throw std::runtime_error("failed to load texture image!");

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        graphics.getDevice().createBuffer(imageSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(graphics.getDevice().getDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
        memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(graphics.getDevice().getDevice(), stagingBufferMemory);

        stbi_image_free(pixels);

        graphics.getDevice().createImage(texWidth, texHeight,
            VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            _textureImage, _textureImageMemory);

        graphics.getDevice().transitionImageLayout(_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        graphics.getDevice().copyBufferToImage(stagingBuffer, _textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        graphics.getDevice().transitionImageLayout(_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(graphics.getDevice().getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(graphics.getDevice().getDevice(), stagingBufferMemory, nullptr);

        _textureImageView = graphics.getDevice().createImageView(_textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
	}

}