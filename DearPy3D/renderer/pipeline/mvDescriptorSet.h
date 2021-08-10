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

		void update(std::vector<VkWriteDescriptorSet>& writeDescriptorSets);

		operator VkDescriptorSet()
		{
			return _descriptorSet;
		}

	private:

		VkDescriptorSet _descriptorSet;

	};

}