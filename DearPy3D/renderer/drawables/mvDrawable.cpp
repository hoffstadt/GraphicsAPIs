#include "mvDrawable.h"
#include "mvGraphics.h"

namespace DearPy3D {

	void mvDrawable::bind(mvGraphics& graphics) const
	{
		auto index = graphics.getCurrentImageIndex();
		_descriptorSets[index]->bind(graphics, *_pipeline);
		_pipeline->bind(graphics);
		_indexBuffer->bind(graphics);
		_vertexBuffer->bind(graphics);
	}

	void mvDrawable::draw(mvGraphics& graphics) const
	{
		graphics.draw(_indexBuffer->getVertexCount());
	}

}