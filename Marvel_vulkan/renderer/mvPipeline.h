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
		void setVertexShader  (mvGraphicsContext& graphics, const std::string& file);
		void setFragmentShader(mvGraphicsContext& graphics, const std::string& file);

	private:


		std::unique_ptr<mvShader> _vertexShader = nullptr;
		std::unique_ptr<mvShader> _fragShader = nullptr;
		mvVertexLayout            _layout;

	};

}