#include "mvSolidSphere.h"
#include "mvGraphics.h"
#include "mvTexture.h"
#include "mvSampler.h"
#include "Sphere.h"
#include <chrono>

namespace DearPy3D {

	mvSolidSphere::mvSolidSphere()
	{

		std::vector<float> vertices;
		std::vector<uint16_t> indices;
		std::vector<float> normals;
		Sphere sphere(0.25f);

		vertices = sphere.vertices;
		indices = sphere.indices;
		normals = sphere.normals;

		std::vector<float> nverticies;
		for (int i = 0; i < vertices.size(); i = i + 3)
		{
			nverticies.push_back(vertices[i]);
			nverticies.push_back(vertices[i + 1]);
			nverticies.push_back(vertices[i + 2]);
			nverticies.push_back(normals[i]);
			nverticies.push_back(normals[i + 1]);
			nverticies.push_back(normals[i + 2]);

			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
			nverticies.push_back(0.0f);
		}

		_vertexBuffer = std::make_shared<mvVertexBuffer>(nverticies);
		_indexBuffer = std::make_shared<mvIndexBuffer>(indices);

		_deletionQueue.pushDeletor([=]() {
			_indexBuffer->cleanup();
			_vertexBuffer->cleanup();
			});
	}

	glm::mat4 mvSolidSphere::getTransform() const
	{
		return glm::translate(glm::vec3{ m_x, m_y, m_z }) *
			glm::rotate(m_xangle, glm::vec3{ 1.0f, 0.0f, 0.0f }) *
			glm::rotate(m_yangle, glm::vec3{ 0.0f, 1.0f, 0.0f }) *
			glm::rotate(m_zangle, glm::vec3{ 0.0f, 0.0f, 1.0f });
	}

	void mvSolidSphere::setPosition(float x, float y, float z)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}

	void mvSolidSphere::setRotation(float x, float y, float z)
	{
		m_xangle = x;
		m_yangle = y;
		m_zangle = z;
	}

}