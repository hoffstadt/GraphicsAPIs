#include "mvTexturedQuad.h"
#include "mvGraphics.h"
#include "mvTexture.h"
#include "mvSampler.h"
#include <chrono>


namespace DearPy3D {

	mvTexturedQuad::mvTexturedQuad(mvGraphics& graphics, const std::string& path)
	{

		_transformBuffer = std::make_unique<mvTransformUniform>(graphics);
		_transformBuffer->_parent = this;

		auto vlayout = mvVertexLayout();
		vlayout.append(ElementType::Position3D);
		vlayout.append(ElementType::Color);
		vlayout.append(ElementType::Texture2D);

		_vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, std::vector<float>{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
		});

		_indexBuffer = std::make_shared<mvIndexBuffer>(graphics, std::vector<uint16_t>{ 
			0u, 1u, 2u, 2u, 3u, 0u
		});

		_sampler = std::make_shared<mvSampler>(graphics);
		_texture = std::make_shared<mvTexture>(graphics, path);

		_descriptorSetLayout = std::make_shared<mvDescriptorSetLayout>();
		_descriptorSetLayout->append(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
		_descriptorSetLayout->append(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
		_descriptorSetLayout->finalize(graphics);

		for (int i = 0; i < 3; i++)
		{
			_descriptorSets.push_back(std::make_shared<mvDescriptorSet>());
			graphics.allocateDescriptorSet(&(*_descriptorSets.back()), *_descriptorSetLayout);
			_descriptorSets.back()->update(graphics, *_transformBuffer->_buf[i], _texture->getImageView(), _sampler->getSampler());
		}

		_pipeline = std::make_shared<mvPipeline>();
		_pipeline->setVertexLayout(vlayout);
		_pipeline->setVertexShader(graphics, "../../DearPy3D/shaders/vert.spv");
		_pipeline->setFragmentShader(graphics, "../../DearPy3D/shaders/frag.spv");
		_pipeline->setDescriptorSetLayout(_descriptorSetLayout);
		_pipeline->setDescriptorSets(_descriptorSets);
		_pipeline->finalize(graphics);
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

	void mvTexturedQuad::bind(mvGraphics& graphics) const
	{
		auto index = graphics.getCurrentImageIndex();
		_transformBuffer->bind(graphics);
		_descriptorSets[index]->bind(graphics, *_pipeline);
		_pipeline->bind(graphics);
		_indexBuffer->bind(graphics);
		_vertexBuffer->bind(graphics);
	}

}