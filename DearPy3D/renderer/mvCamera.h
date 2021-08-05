#pragma once

#include "mvMath.h"

namespace DearPy3D {

	class mvGraphics;

	class mvCamera 
	{

	public:

		mvCamera(mvGraphics& graphics, float width, float height, glm::vec3 homePos = { 0.0f,0.0f, 0.0f });

		void rotate(float dx, float dy);
		void translate(float dx, float dy, float dz);
		void setWidth(float width) { _width = width; }
		void setHeight(float height) { _height = height; }

		glm::mat4 getMatrix() const;
		glm::mat4 getProjection() const;

		void bind(mvGraphics& graphics);

	private:

		glm::vec3 _pos;
		float     _pitch = 0.0f;
		float     _yaw = 0.0f;
		float     _width = 0.0f;
		float     _height = 0.0f;

		static constexpr float s_travelSpeed = 12.0f;
		static constexpr float s_rotationSpeed = 0.004f;

	};

}