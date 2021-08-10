#pragma once

#include <memory>
#include "mvMath.h"
#include "mvVertexBuffer.h"
#include "mvIndexBuffer.h"
#include "mvPipeline.h"
#include "mvDescriptorSet.h"
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
		void draw() const;
		void setMaterial(std::shared_ptr<mvMaterial> material) { _material = material; }

	protected:

		std::shared_ptr<mvMaterial>     _material;
		mvDeletionQueue                 _deletionQueue;
		std::shared_ptr<mvIndexBuffer>  _indexBuffer;
		std::shared_ptr<mvVertexBuffer> _vertexBuffer;

	};

}