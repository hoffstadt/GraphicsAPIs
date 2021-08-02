#include "mvDescriptorPool.h"
#include <stdexcept>
#include "mvGraphicsContext.h"
#include "mvDescriptorSet.h"

namespace Marvel {

	mvDescriptorPool::mvDescriptorPool(mvGraphicsContext& graphics)
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

	void mvDescriptorPool::allocateDescriptorSet(mvGraphicsContext& graphics, mvDescriptorSet* descriptorSet, mvDescriptorSetLayout& layout)
	{
		std::vector<VkDescriptorSetLayout> layouts(1, layout._layout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = layouts.data();

		if (vkAllocateDescriptorSets(graphics.getDevice().getDevice(), &allocInfo, &descriptorSet->_descriptorSet) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate descriptor sets!");
	}

	void mvDescriptorPool::cleanup(mvGraphicsContext& graphics)
	{
		vkDestroyDescriptorPool(graphics.getDevice().getDevice(), _descriptorPool, nullptr);
	}
}