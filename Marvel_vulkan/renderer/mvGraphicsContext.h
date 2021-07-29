#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"
#include "mvPipeline.h"
#include "mvDevice.h"

namespace Marvel{

	class mvDevice;

	class mvGraphicsContext
	{

	public:

		mvGraphicsContext(GLFWwindow* window);

		void begin();
		void end();

		void setPipeline    (std::shared_ptr<mvPipeline>);
		void setVertexBuffer(std::shared_ptr<mvVertexBuffer>);
		void setIndexBuffer (std::shared_ptr<mvIndexBuffer>);

		mvDevice&   getDevice();
		mvPipeline& getPipeline();

		// filled by device
		mvVertexBuffer&               getVertexBuffer();
		mvIndexBuffer&                getIndexBuffer();

	private:

		mvDevice                        _device;
		std::shared_ptr<mvVertexBuffer> _vertexBuffer = nullptr;
		std::shared_ptr<mvIndexBuffer>  _indexBuffer  = nullptr;
		std::shared_ptr<mvPipeline>     _pipeline     = nullptr;
		
	};

}