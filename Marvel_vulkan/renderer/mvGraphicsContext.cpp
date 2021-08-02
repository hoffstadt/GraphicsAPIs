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

	void mvGraphicsContext::beginpresent()
	{
		_device->beginpresent(*this);
	}

	void mvGraphicsContext::endpresent(std::vector<std::shared_ptr<mvCommandBuffer>>& commandBuffers)
	{
		_device->endpresent(*this, commandBuffers);
	}
}