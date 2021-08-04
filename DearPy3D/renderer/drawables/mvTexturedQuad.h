#pragma once
#include <string>
#include "mvBuffer.h"
#include "mvMath.h"
#include "mvDrawable.h"

namespace DearPy3D {

	struct mvTransforms
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	// forward declarations
	class mvGraphics;
	class mvTexture;
	class mvSampler;

	class mvTexturedQuad : public mvDrawable
	{

	public:

		mvTexturedQuad(mvGraphics& graphics, const std::string& path);

		void update(mvGraphics& graphics);

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

		std::shared_ptr<mvTexture> _texture;
		std::shared_ptr<mvSampler> _sampler;
		std::vector<std::shared_ptr<mvBuffer<mvTransforms>>> _uniformBuffers;
	};

}