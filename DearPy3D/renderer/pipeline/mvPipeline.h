#pragma once

#include <memory>
#include <vulkan/vulkan.h>
#include "mvShader.h"
#include "mvVertexLayout.h"

namespace DearPy3D {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphics;

	//---------------------------------------------------------------------
	// mvPipeline
	//---------------------------------------------------------------------
	class mvPipeline
	{

	public:

		void bind                   (uint32_t index);
		void finalize               ();
		void setDescriptorSets      (std::vector<VkDescriptorSet>);
		void setDescriptorSetLayouts(std::vector<VkDescriptorSetLayout>);
		void setVertexLayout        (mvVertexLayout);
		void setVertexShader        (const std::string& file);
		void setFragmentShader      (const std::string& file);
		void finish                 ();

		VkPipelineLayout getLayout() { return _pipelineLayout; }

	private:

		std::unique_ptr<mvShader>          _vertexShader = nullptr;
		std::unique_ptr<mvShader>          _fragShader = nullptr;
		mvVertexLayout                     _layout;
		std::vector<VkDescriptorSetLayout> _descriptorSetLayouts;
		std::vector<VkDescriptorSet>       _descriptorSets;
		VkPipelineLayout                   _pipelineLayout = nullptr;
		VkPipeline                         _pipeline = nullptr;

	};

}