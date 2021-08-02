#pragma once

#include <memory>
#include <vulkan/vulkan.h>
#include "mvShader.h"
#include "mvVertexLayout.h"
#include "mvDescriptorSet.h"

namespace Marvel {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphicsContext;

	//---------------------------------------------------------------------
	// mvPipeline
	//---------------------------------------------------------------------
	class mvPipeline
	{

	public:

		void bind                   (mvGraphicsContext& graphics);
		void finalize               (mvGraphicsContext& graphics);
		void setDescriptorSet       (std::shared_ptr<mvDescriptorSet>);
		void setDescriptorSetLayout (std::shared_ptr<mvDescriptorSetLayout>);
		void setVertexLayout        (mvVertexLayout);
		void setVertexShader        (mvGraphicsContext& graphics, const std::string& file);
		void setFragmentShader      (mvGraphicsContext& graphics, const std::string& file);
		void finish                 (mvGraphicsContext&);

	private:

		std::unique_ptr<mvShader>              _vertexShader = nullptr;
		std::unique_ptr<mvShader>              _fragShader = nullptr;
		mvVertexLayout                         _layout;
		std::shared_ptr<mvDescriptorSetLayout> _descriptorSetLayout = nullptr;
		std::shared_ptr<mvDescriptorSet>       _descriptorSet = nullptr;
		VkPipelineLayout                       _pipelineLayout = nullptr;
		VkPipeline                             _pipeline = nullptr;

	};

}