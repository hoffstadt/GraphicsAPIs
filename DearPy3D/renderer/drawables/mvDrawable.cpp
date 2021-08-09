#include "mvDrawable.h"
#include "mvGraphics.h"

namespace DearPy3D {

	void mvDrawable::cleanup()
	{
		vkDeviceWaitIdle(mvGraphics::GetContext().getLogicalDevice());
		_deletionQueue.flush();
	}

	void mvDrawable::bind() const
	{
		auto index = mvGraphics::GetContext().getSwapChain().getCurrentImageIndex();
		_descriptorSets[index]->bind(*_pipeline);
		_pipeline->bind();
		_indexBuffer->bind();
		_vertexBuffer->bind();
	}

	void mvDrawable::draw() const
	{
		mvGraphics::GetContext().getSwapChain().draw(_indexBuffer->getVertexCount());
	}

}