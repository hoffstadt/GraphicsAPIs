#pragma once

#include "mvMath.h"

namespace DearPy3D {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphics;
	class mvDrawable;
	class mvMaterial;
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

		void setCamera(mvCamera& camera);

		void renderDrawable(mvDrawable& drawable, mvMaterial& material);

	private:

		Transforms _transforms;
		glm::mat4  _camera;
		glm::mat4  _projection;

	};

}