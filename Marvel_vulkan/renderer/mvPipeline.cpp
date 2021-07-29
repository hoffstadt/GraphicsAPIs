#include "mvPipeline.h"
#include "mvDevice.h"

namespace Marvel {

	mvPipeline::mvPipeline(mvDevice& device)
	{
		_device = device.getDevice();
	}

	mvPipeline::~mvPipeline()
	{
		vkDestroyPipeline(_device, _pipeline, nullptr);
		vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
	}

	void mvPipeline::setVertexLayout(mvVertexLayout layout)
	{
		_layout = layout;
	}

	void mvPipeline::setVertexShader(std::shared_ptr<mvShader> shader)
	{
		_vertexShader = shader;
	}

	void mvPipeline::setFragmentShader(std::shared_ptr<mvShader> shader)
	{
		_fragShader = shader;
	}

	VkPipelineLayout* mvPipeline::getPipelineLayout()
	{
		return &_pipelineLayout;
	}

	VkPipeline* mvPipeline::getPipeline()
	{
		return &_pipeline;
	}

	const mvVertexLayout& mvPipeline::getVertexLayout()
	{
		return _layout;
	}

	const mvShader& mvPipeline::getVertexShader()
	{
		return *_vertexShader;
	}

	const mvShader& mvPipeline::getFragmentShader()
	{
		return *_fragShader;
	}

}