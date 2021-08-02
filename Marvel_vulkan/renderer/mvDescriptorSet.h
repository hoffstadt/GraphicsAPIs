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

		void update(mvGraphicsContext& graphics);

	private:

		std::vector<VkDescriptorSet> _descriptorSets;

	};

	//-----------------------------------------------------------------------------
	// mvDescriptorPool
	//-----------------------------------------------------------------------------
	class mvDescriptorPool
	{

	public:

		std::shared_ptr<mvDescriptorSet> allocateDescriptorSets(mvGraphicsContext& graphics, mvDescriptorSetLayout& layout);

		void finalize(mvGraphicsContext& graphics);

	private:

		VkDescriptorPool _descriptorPool;

	};

}