#pragma once

#include <memory>
#include <vector>
#include "mvBuffer.h"
#include "mvMath.h"

namespace DearPy3D {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvDrawable;
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvTransformUniform
	//-----------------------------------------------------------------------------
	class mvTransformUniform
	{
		friend class mvTexturedQuad;
		friend class mvCube;
		friend class mvSolidSphere;

		struct Transforms
		{
			glm::mat4 model = glm::identity<glm::mat4>();
			glm::mat4 modelView = glm::identity<glm::mat4>();
			glm::mat4 modelViewProjection = glm::identity<glm::mat4>();
		};

	public:

		mvTransformUniform();

		void bind();
		void cleanup();

	private:

		mvDrawable* _parent = nullptr;
		std::vector<std::unique_ptr<mvBuffer<Transforms>>> _buf;
		Transforms _transforms = {};

	};

}