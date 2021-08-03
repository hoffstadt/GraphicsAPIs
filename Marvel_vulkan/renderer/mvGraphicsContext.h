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

		mvDevice&   getDevice();
		void        begin();
		void        submit(mvCommandBuffer& commandBuffer);
		void        present();

	private:

		std::unique_ptr<mvDevice> _device= nullptr;
		
	};

}