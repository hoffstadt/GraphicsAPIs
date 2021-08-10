#pragma once
#include <string>
#include "mvBuffer.h"
#include "mvMath.h"
#include "mvDrawable.h"

namespace DearPy3D {

	// forward declarations
	class mvGraphics;

	class mvSolidSphere : public mvDrawable
	{

	public:

		mvSolidSphere();

		glm::mat4 getTransform() const override;

		void setPosition(float x, float y, float z);
		void setRotation(float x, float y, float z);

	private:

		float m_x = 0.0f;
		float m_y = 0.0f;
		float m_z = 0.0f;
		float m_xangle = 0.0f;
		float m_yangle = 0.0f;
		float m_zangle = 0.0f;

	};

}