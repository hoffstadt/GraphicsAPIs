#pragma once

#include <memory>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"
#include "mvPipeline.h"

namespace Marvel{

	class mvDevice;

	class mvGraphicsContext
	{

	public:

		mvGraphicsContext(mvDevice&);
		~mvGraphicsContext();

		void begin();
		void end();

		void setPipeline    (std::shared_ptr<mvPipeline>);
		void setVertexBuffer(std::shared_ptr<mvVertexBuffer>);
		void setIndexBuffer (std::shared_ptr<mvIndexBuffer>);

		mvPipeline& getPipeline();

		// filled by device
		VkCommandPool*                getCommandPool();
		std::vector<VkCommandBuffer>& getCommandBuffers();
		mvVertexBuffer&               getVertexBuffer();
		mvIndexBuffer&                getIndexBuffer();

	private:

		VkDevice                        _device;
		mvVertexLayout                  _vertexLayout;
		std::shared_ptr<mvVertexBuffer> _vertexBuffer = nullptr;
		std::shared_ptr<mvIndexBuffer>  _indexBuffer  = nullptr;
		std::shared_ptr<mvPipeline>     _pipeline     = nullptr;
		VkCommandPool                   _commandPool;
		std::vector<VkCommandBuffer>    _commandBuffers;
	};

}