#pragma once

#include <memory>
#include "mvMath.h"
#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"
#include "mvPipeline.h"
#include "mvDescriptorSet.h"
#include "mvDeletionQueue.h"

namespace DearPy3D {

	class mvGraphics;

	class mvDrawable
	{

	public:

		mvDrawable() = default;
		virtual ~mvDrawable() {}

		void cleanup(mvGraphics& graphics);

		virtual glm::mat4 getTransform() const = 0;

		virtual void bind(mvGraphics& graphics) const;
		void draw(mvGraphics & graphics) const;

	protected:

		mvDeletionQueue                               _deletionQueue;
		std::shared_ptr<mvPipeline>                   _pipeline;
		std::shared_ptr<mvDescriptorSetLayout>        _descriptorSetLayout;
		std::vector<std::shared_ptr<mvDescriptorSet>> _descriptorSets;
		std::shared_ptr<mvIndexBuffer>                _indexBuffer;
		std::shared_ptr<mvVertexBuffer>               _vertexBuffer;

	};

}