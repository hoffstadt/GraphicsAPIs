#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>

namespace Marvel {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphics;
	class mvDescriptorSet;
	class mvDescriptorSetLayout;

	//-----------------------------------------------------------------------------
	// mvDescriptorPool
	//-----------------------------------------------------------------------------
	class mvDescriptorPool
	{

	public:

		mvDescriptorPool(mvGraphics& graphics);

		void cleanup(mvGraphics& graphics);

		void allocateDescriptorSet(mvGraphics& graphics, mvDescriptorSet* descriptorSet, mvDescriptorSetLayout& layout);

	private:

		VkDescriptorPool _descriptorPool;

	};

}