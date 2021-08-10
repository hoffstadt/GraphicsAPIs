#include "mvTexturedQuad.h"
#include "mvGraphics.h"
#include "mvTexture.h"
#include "mvSampler.h"
#include <chrono>

namespace DearPy3D {

	mvTexturedQuad::mvTexturedQuad(const std::string& path)
	{

		// initialize vertices
		auto vertices = std::vector<float>
		{
			-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		};

		// initialize indexes
		auto indices = std::vector<uint16_t>{0u, 2u, 1u, 0u, 1u, 3u};

		for (size_t i = 0; i < indices.size(); i += 3)
		{
			auto v0 = vertices[14 * indices[i]];
			auto v1 = vertices[14 * indices[i + 1]];
			auto v2 = vertices[14 * indices[i + 2]];
			const auto p0 = glm::vec3{ v0, vertices[14 * indices[i] + 1], vertices[14 * indices[i] + 2] };
			const auto p1 = glm::vec3{ v1, vertices[14 * indices[i + 1] + 1], vertices[14 * indices[i + 1] + 2] };
			const auto p2 = glm::vec3{ v2, vertices[14 * indices[i + 2] + 1], vertices[14 * indices[i + 2] + 2] };

			const auto n = glm::normalize(glm::cross(p1 - p0, p2 - p0));
			vertices[14 * indices[i] + 3] = n[0];
			vertices[14 * indices[i] + 4] = n[1];
			vertices[14 * indices[i] + 5] = n[2];
			vertices[14 * indices[i + 1] + 3] = n[0];
			vertices[14 * indices[i + 1] + 4] = n[1];
			vertices[14 * indices[i + 1] + 5] = n[2];
			vertices[14 * indices[i + 2] + 3] = n[0];
			vertices[14 * indices[i + 2] + 4] = n[1];
			vertices[14 * indices[i + 2] + 5] = n[2];
		}

		_vertexBuffer = std::make_shared<mvVertexBuffer>(vertices);
		_indexBuffer = std::make_shared<mvIndexBuffer>(indices);

		_deletionQueue.pushDeletor([=]() {
			_indexBuffer->cleanup();
			_vertexBuffer->cleanup();
			});
	}

	glm::mat4 mvTexturedQuad::getTransform() const
	{
		return glm::translate(glm::vec3{ m_x, m_y, m_z }) *
			glm::rotate(m_xangle, glm::vec3{ 1.0f, 0.0f, 0.0f }) *
			glm::rotate(m_yangle, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m_zangle, glm::vec3{ 0.0f, 0.0f, 1.0f });
	}

	void mvTexturedQuad::setPosition(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	void mvTexturedQuad::setRotation(float x, float y, float z)
	{
		m_xangle = x;
		m_yangle = y;
		m_zangle = z;
	}

}