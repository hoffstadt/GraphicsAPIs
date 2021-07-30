#pragma once

#include <memory>
#include <vulkan/vulkan.h>
#include "mvShader.h"
#include "mvVertexLayout.h"

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

		void bind             (mvGraphicsContext& graphics);
		void finalize         (mvGraphicsContext& graphics);
		void setVertexLayout  (mvVertexLayout);
		void setVertexShader  (mvGraphicsContext& graphics, const std::string& file);
		void setFragmentShader(mvGraphicsContext& graphics, const std::string& file);
		void finish           (mvGraphicsContext&);

	private:

		std::unique_ptr<mvShader> _vertexShader = nullptr;
		std::unique_ptr<mvShader> _fragShader = nullptr;
		mvVertexLayout            _layout;
		VkPipelineLayout          _pipelineLayout;
		VkPipeline                _pipeline;

	};

}