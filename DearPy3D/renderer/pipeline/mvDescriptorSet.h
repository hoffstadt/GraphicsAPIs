#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace DearPy3D {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphics;
	class mvDescriptorSet;
	class mvPipeline;

	template<typename T>
	class mvBuffer;

	//-----------------------------------------------------------------------------
	// mvDescriptorSetLayout
	//-----------------------------------------------------------------------------
	class mvDescriptorSetLayout
	{

		friend class mvGraphics;

	public:

		void append(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags);
		void finalize();
		const VkDescriptorSetLayout* getLayout() const { return &_layout; }
		void cleanup();

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
		friend class mvGraphics;

	public:

		void bind(mvPipeline& pipeline);

		template<typename T>
		void update(mvBuffer<T>& buffer, VkImageView imageview, VkSampler sampler)
		{

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = buffer.getBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(T);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = imageview;
			imageInfo.sampler = sampler;

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

			vkUpdateDescriptorSets(mvGraphics::GetContext().getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		}

	private:

		VkDescriptorSet _descriptorSet;

	};

}