#pragma once

#include <memory>
#include "mvMath.h"
#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"
#include "mvPipeline.h"
#include "mvDeletionQueue.h"
#include "mvMaterial.h"

namespace DearPy3D {

	class mvGraphics;

	class mvDrawable
	{

	public:

		mvDrawable() = default;
		virtual ~mvDrawable() {}

		void cleanup();

		virtual glm::mat4 getTransform() const = 0;

		void bind() const;
		uint32_t getVertexCount() const;

	protected:

		mvDeletionQueue                 _deletionQueue;
		std::shared_ptr<mvIndexBuffer>  _indexBuffer;
		std::shared_ptr<mvVertexBuffer> _vertexBuffer;

	};

}