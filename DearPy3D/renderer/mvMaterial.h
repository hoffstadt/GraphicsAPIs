#pragma once

#include <memory>
#include "mvDescriptorSet.h"
#include "mvPipeline.h"
#include "mvDeletionQueue.h"

namespace DearPy3D {

	class mvTexture;
	class mvSampler;
	class mvDrawable;

	class mvMaterial
	{

	public:

		mvMaterial();

		void bind();
		void cleanup();
		mvPipeline& getPipeline() const { return *_pipeline; }

	private:

		mvDeletionQueue                               _deletionQueue;
		std::shared_ptr<mvPipeline>                   _pipeline;
		std::shared_ptr<mvDescriptorSetLayout>        _descriptorSetLayout;
		std::vector<std::shared_ptr<mvDescriptorSet>> _descriptorSets;
		std::shared_ptr<mvTexture>                    _texture;
		std::shared_ptr<mvSampler>                    _sampler;
	};

}