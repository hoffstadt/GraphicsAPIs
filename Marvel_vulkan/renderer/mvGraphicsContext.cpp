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

	void mvGraphicsContext::beginRecording(int buffer)
	{
		_device->beginRecording(buffer);
	}

	void mvGraphicsContext::endRecording()
	{
		_device->endRecording();
	}

	void mvGraphicsContext::draw(uint32_t vertexCount)
	{
		_device->draw(vertexCount);
	}

	void mvGraphicsContext::beginpresent()
	{
		_device->beginpresent(*this);
	}

	void mvGraphicsContext::endpresent()
	{
		_device->endpresent(*this);
	}
}