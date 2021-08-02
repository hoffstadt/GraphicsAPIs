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
	class mvDescriptorSetLayout;

	//-----------------------------------------------------------------------------
	// mvDescriptorPool
	//-----------------------------------------------------------------------------
	class mvDescriptorPool
	{

	public:

		mvDescriptorPool(mvGraphicsContext& graphics);

		void cleanup(mvGraphicsContext& graphics);

		void allocateDescriptorSet(mvGraphicsContext& graphics, mvDescriptorSet* descriptorSet, mvDescriptorSetLayout& layout);

	private:

		VkDescriptorPool _descriptorPool;

	};

}