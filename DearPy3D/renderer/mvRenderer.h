#pragma once

#include <vulkan/vulkan.h>
#include "mvMath.h"

namespace DearPy3D {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphics;
	class mvDrawable;
	class mvPipeline;
	class mvCamera;

	//---------------------------------------------------------------------
	// mvRenderer
	//---------------------------------------------------------------------
	class mvRenderer
	{

		struct Transforms
		{
			glm::mat4 model = glm::identity<glm::mat4>();
			glm::mat4 modelView = glm::identity<glm::mat4>();
			glm::mat4 modelViewProjection = glm::identity<glm::mat4>();
		};

	public:

		void beginFrame();
		void endFrame();

		void beginPass(VkCommandBuffer commandBuffer, VkRenderPass renderPass);
		void endPass(VkCommandBuffer commandBuffer);

		void setCamera(mvCamera& camera);

		void renderDrawable(mvDrawable& drawable, mvPipeline& pipeline);

	private:

		Transforms _transforms;
		glm::mat4  _camera;
		glm::mat4  _projection;

	};

}