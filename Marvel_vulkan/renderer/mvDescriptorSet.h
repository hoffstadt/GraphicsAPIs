#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace Marvel {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphicsContext;
	class mvDescriptorSet;

	template<typename T>
	class mvBuffer;

	//-----------------------------------------------------------------------------
	// mvDescriptorSetLayout
	//-----------------------------------------------------------------------------
	class mvDescriptorSetLayout
	{

		friend class mvDescriptorPool;

	public:

		void append(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags);
		void finalize(mvGraphicsContext& graphics);
		const VkDescriptorSetLayout* getLayout() const { return &_layout; }

	private:

		std::vector<VkDescriptorSetLayoutBinding> _bindings;
		VkDescriptorSetLayout                     _layout = nullptr;

	};

	//-----------------------------------------------------------------------------
	// mvDescriptorSet
	//-----------------------------------------------------------------------------
	class mvDescriptorSet
	{

		friend class mvPipeline;
		friend class mvDescriptorPool;

	public:

		template<typename T>
		void update(mvGraphicsContext& graphics, mvBuffer<T>& buffer)
		{

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = buffer.getBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = graphics.getDevice().getTextureImageView();
			imageInfo.sampler = graphics.getDevice().getTextureSampler();

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = _descriptorSet;
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = _descriptorSet;
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(graphics.getDevice().getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		}

	private:

		VkDescriptorSet _descriptorSet;

	};

}