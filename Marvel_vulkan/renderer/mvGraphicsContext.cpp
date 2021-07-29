#include "mvGraphicsContext.h"
#include "mvDevice.h"

namespace Marvel {

	mvGraphicsContext::mvGraphicsContext(mvDevice& device)
	{
		_device = device.getDevice();
	}

	mvGraphicsContext::~mvGraphicsContext()
	{
		vkDestroyCommandPool(_device, _commandPool, nullptr);
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
		_vertexLayout = buffer->GetLayout();
	}

	void mvGraphicsContext::setIndexBuffer(std::shared_ptr<mvIndexBuffer> buffer)
	{
		_indexBuffer = buffer;
	}

	VkCommandPool* mvGraphicsContext::getCommandPool()
	{
		return &_commandPool;
	}

	std::vector<VkCommandBuffer>& mvGraphicsContext::getCommandBuffers()
	{
		return _commandBuffers;
	}

	mvPipeline& mvGraphicsContext::getPipeline()
	{
		return *_pipeline;
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