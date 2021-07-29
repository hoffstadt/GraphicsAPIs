#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mvShader.h"
#include "mvVertexLayout.h"

namespace Marvel {

	class mvGraphicsContext;

	class mvPipeline
	{

	public:

		void create(mvGraphicsContext& graphics);

		void setVertexLayout  (mvVertexLayout);
		void setVertexShader  (std::shared_ptr<mvShader>);
		void setFragmentShader(std::shared_ptr<mvShader>);

		const mvVertexLayout& getVertexLayout();
		const mvShader&       getVertexShader();
		const mvShader&       getFragmentShader();

	private:


		std::shared_ptr<mvShader> _vertexShader = nullptr;
		std::shared_ptr<mvShader> _fragShader = nullptr;
		mvVertexLayout            _layout;

	};

}