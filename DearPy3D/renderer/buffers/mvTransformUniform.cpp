#include "mvTransformUniform.h"
#include "mvGraphics.h"
#include "mvDrawable.h"

namespace DearPy3D {

	mvTransformUniform::mvTransformUniform()
	{
		for (int i = 0; i < 3; i++)
			_buf.push_back(std::make_unique<mvBuffer<Transforms>>(
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

	}

	void mvTransformUniform::bind()
	{
		auto model = _parent->getTransform();
		auto modelView = mvGraphics::GetContext().getCamera() * _parent->getTransform();
		auto modelViewProj = mvGraphics::GetContext().getProjection() * mvGraphics::GetContext().getCamera() * _parent->getTransform();

		_transforms.model = model;
		_transforms.modelView = modelView;
		_transforms.modelViewProjection = modelViewProj;

		auto index = mvGraphics::GetContext().getSwapChain().getCurrentImageIndex();
		_buf[index]->update( _transforms);
	}

	void mvTransformUniform::cleanup()
	{
		for (auto& item : _buf)
			item->cleanup();
	}
}