#include "mvDrawable.h"
#include "mvGraphics.h"

namespace DearPy3D {

	mvDrawable::~mvDrawable()
	{
		//std::shared_ptr<mvPipeline>                   _pipeline;
		//std::shared_ptr<mvDescriptorSetLayout>        _descriptorSetLayout;
		//std::vector<std::shared_ptr<mvDescriptorSet>> _descriptorSets;
		//std::shared_ptr<mvIndexBuffer>                _indexBuffer;
		//std::shared_ptr<mvVertexBuffer>               _vertexBuffer;
	}

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