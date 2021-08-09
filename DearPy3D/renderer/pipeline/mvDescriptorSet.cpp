#include "mvDescriptorSet.h"
#include <stdexcept>
#include "mvGraphics.h"
#include "mvPipeline.h"

namespace DearPy3D {

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

	void mvDescriptorSetLayout::cleanup(mvGraphics& graphics)
	{
		vkDestroyDescriptorSetLayout(graphics.getLogicalDevice(), _layout, nullptr);
	}

	void mvDescriptorSetLayout::finalize(mvGraphics& graphics)
	{
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(_bindings.size());
		layoutInfo.pBindings = _bindings.data();

		if (vkCreateDescriptorSetLayout(graphics.getLogicalDevice(), &layoutInfo, nullptr, &_layout) != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor set layout!");
	}

	void mvDescriptorSet::bind(mvGraphics& graphics, mvPipeline& pipeline)
	{
		vkCmdBindDescriptorSets(graphics.getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipeline.getLayout(), 0, 1, &_descriptorSet, 0, nullptr);
	}

}