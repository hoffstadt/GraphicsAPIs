#include "mvGraphicsContext.h"
#include "mvDevice.h"

namespace Marvel {

	mvGraphicsContext::mvGraphicsContext(GLFWwindow* window)
	{
		_device = std::make_unique<mvDevice>(window);
	}

	mvDevice& mvGraphicsContext::getDevice()
	{
		return *_device;
	}

	void mvGraphicsContext::present()
	{
		_device->present(*this);
	}

	void mvGraphicsContext::begin()
	{
		_device->begin(*this);
	}

	void mvGraphicsContext::submit(mvCommandBuffer& commandBuffer)
	{
		_device->submit(commandBuffer);
	}
}