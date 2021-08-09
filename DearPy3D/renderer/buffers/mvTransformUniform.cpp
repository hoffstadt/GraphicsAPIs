#include "mvTransformUniform.h"
#include "mvGraphics.h"
#include "mvDrawable.h"

namespace DearPy3D {

	mvTransformUniform::mvTransformUniform(mvGraphics& graphics)
	{
		for (int i = 0; i < 3; i++)
			_buf.push_back(std::make_unique<mvBuffer<Transforms>>(graphics,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

	}

	void mvTransformUniform::bind(mvGraphics& graphics)
	{
		auto model = _parent->getTransform();
		auto modelView = graphics.getCamera() * _parent->getTransform();
		auto modelViewProj = graphics.getProjection() * graphics.getCamera() * _parent->getTransform();

		_transforms.model = model;
		_transforms.modelView = modelView;
		_transforms.modelViewProjection = modelViewProj;

		auto index = graphics.getSwapChain().getCurrentImageIndex();
		_buf[index]->update(graphics, _transforms);
	}

	void mvTransformUniform::cleanup(mvGraphics& graphics)
	{
		for (auto& item : _buf)
			item->cleanup(graphics);
	}
}