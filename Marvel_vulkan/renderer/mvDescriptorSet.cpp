#include "mvDescriptorSet.h"
#include <stdexcept>
#include "mvGraphicsContext.h"

namespace Marvel {

	void mvDescriptorSetLayout::append(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stageFlags)
	{
		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorType = descriptorType;
		layoutBinding.descriptorCount = 1;
		layoutBinding.stageFlags = stageFlags;
		layoutBinding.pImmutableSamplers = nullptr; // Optional
		_bindings.push_back(layoutBinding);

	}

	void mvDescriptorSetLayout::finalize(mvGraphicsContext& graphics)
	{
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(_bindings.size());
		layoutInfo.pBindings = _bindings.data();

		if (vkCreateDescriptorSetLayout(graphics.getDevice().getDevice(), &layoutInfo, nullptr, &_layout) != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor set layout!");
	}

	void mvDescriptorSet::update(mvGraphicsContext& graphics)
	{
		for (size_t i = 0; i < graphics.getDevice().getSwapChainImageCount(); i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = graphics.getDevice().getUniformBuffers()[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = graphics.getDevice().getTextureImageView();
			imageInfo.sampler = graphics.getDevice().getTextureSampler();

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = _descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = _descriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(graphics.getDevice().getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}

	void mvDescriptorPool::finalize(mvGraphicsContext& graphics)
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(graphics.getDevice().getSwapChainImageCount());
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(graphics.getDevice().getSwapChainImageCount());

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(graphics.getDevice().getSwapChainImageCount());

		if (vkCreateDescriptorPool(graphics.getDevice().getDevice(), &poolInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor pool!");
	}

	std::shared_ptr<mvDescriptorSet> mvDescriptorPool::allocateDescriptorSets(mvGraphicsContext& graphics, mvDescriptorSetLayout& layout)
	{
		std::vector<VkDescriptorSetLayout> layouts(graphics.getDevice().getSwapChainImageCount(), layout._layout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(graphics.getDevice().getSwapChainImageCount());
		allocInfo.pSetLayouts = layouts.data();


		auto descriptorSet = std::make_shared<mvDescriptorSet>();

		descriptorSet->_descriptorSets.resize(graphics.getDevice().getSwapChainImageCount());
		if (vkAllocateDescriptorSets(graphics.getDevice().getDevice(), &allocInfo, descriptorSet->_descriptorSets.data()) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate descriptor sets!");
	
		return descriptorSet;
	}

}