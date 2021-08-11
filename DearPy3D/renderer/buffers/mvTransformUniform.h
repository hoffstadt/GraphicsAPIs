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
		friend class mvMaterial;

	public:

		struct Transforms
		{
			glm::mat4 model = glm::identity<glm::mat4>();
			glm::mat4 modelView = glm::identity<glm::mat4>();
			glm::mat4 modelViewProjection = glm::identity<glm::mat4>();
		};


	public:

		void bind();

		const Transforms& getTransforms() const { return _transforms; }

	private:

		const mvDrawable* _parent = nullptr;
		Transforms _transforms = {};

	};

}