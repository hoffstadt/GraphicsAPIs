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

}