#include "mvGraphicsContext.h"
#include "mvDevice.h"

namespace Marvel {

	mvGraphicsContext::mvGraphicsContext(GLFWwindow* window)
	{
		_device.init(window);
	}

	void mvGraphicsContext::begin()
	{

	}

	void mvGraphicsContext::end()
	{

	}

	void mvGraphicsContext::setPipeline(std::shared_ptr<mvPipeline> pipeline)
	{
		_pipeline = pipeline;
	}

	void mvGraphicsContext::setVertexBuffer(std::shared_ptr<mvVertexBuffer> buffer)
	{
		_vertexBuffer = buffer;
	}

	void mvGraphicsContext::setIndexBuffer(std::shared_ptr<mvIndexBuffer> buffer)
	{
		_indexBuffer = buffer;
	}

	mvPipeline& mvGraphicsContext::getPipeline()
	{
		return *_pipeline;
	}

	mvDevice& mvGraphicsContext::getDevice()
	{
		return _device;
	}

	mvVertexBuffer& mvGraphicsContext::getVertexBuffer()
	{
		return *_vertexBuffer;
	}

	mvIndexBuffer& mvGraphicsContext::getIndexBuffer()
	{
		return *_indexBuffer;
	}

}