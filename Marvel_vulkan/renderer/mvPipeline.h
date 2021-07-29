#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mvShader.h"
#include "mvVertexLayout.h"

namespace Marvel {

	class mvDevice;

	class mvPipeline
	{

	public:

		mvPipeline(mvDevice& device);
		~mvPipeline();

		void setVertexLayout  (mvVertexLayout);
		void setVertexShader  (std::shared_ptr<mvShader>);
		void setFragmentShader(std::shared_ptr<mvShader>);

		const mvVertexLayout& getVertexLayout();
		const mvShader&       getVertexShader();
		const mvShader&       getFragmentShader();

		// filled by device
		VkPipelineLayout* getPipelineLayout();
		VkPipeline*       getPipeline();

	private:

		VkDevice                  _device;
		VkPipelineLayout          _pipelineLayout;
		VkPipeline                _pipeline;
		std::shared_ptr<mvShader> _vertexShader = nullptr;
		std::shared_ptr<mvShader> _fragShader = nullptr;
		mvVertexLayout            _layout;

	};

}