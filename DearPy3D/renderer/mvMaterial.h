#pragma once

#include <memory>
#include "mvPipeline.h"
#include "mvDeletionQueue.h"
#include "mvMaterialBuffer.h"

namespace DearPy3D {

	class mvTexture;
	class mvSampler;
	class mvDrawable;

	class mvMaterial
	{

	public:

		mvMaterial();

		void bind(uint32_t index, mvMaterialBuffer::mvMaterialData data);
		void cleanup();
		mvPipeline& getPipeline() const { return *_pipeline; }
		void setOffsetIndex(uint32_t index) { _offsetIndex = index; }

	private:

		mvDeletionQueue                    _deletionQueue;
		std::shared_ptr<mvPipeline>        _pipeline;
		std::shared_ptr<mvTexture>         _texture;
		std::shared_ptr<mvSampler>         _sampler;
		uint32_t                           _offsetIndex = 0u;
		std::shared_ptr<mvMaterialBuffer>  _materialBuffer;

	};

}