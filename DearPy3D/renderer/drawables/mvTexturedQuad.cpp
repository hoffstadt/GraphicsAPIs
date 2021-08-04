#include "mvTexturedQuad.h"
#include "mvGraphics.h"
#include "mvTexture.h"
#include "mvSampler.h"
#include <chrono>


namespace DearPy3D {

	mvTexturedQuad::mvTexturedQuad(mvGraphics& graphics, const std::string& path)
	{

		auto vlayout = mvVertexLayout();
		vlayout.append(ElementType::Position3D);
		vlayout.append(ElementType::Color);
		vlayout.append(ElementType::Texture2D);

		for (int i = 0; i < 3; i++)
			_uniformBuffers.push_back(std::make_shared<mvBuffer<mvTransforms>>(graphics, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));


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
			_descriptorSets.back()->update(graphics, *_uniformBuffers[i], _texture->getImageView(), _sampler->getSampler());
		}

		_pipeline = std::make_shared<mvPipeline>();
		_pipeline->setVertexLayout(vlayout);
		_pipeline->setVertexShader(graphics, "../../DearPy3D/shaders/vert.spv");
		_pipeline->setFragmentShader(graphics, "../../DearPy3D/shaders/frag.spv");
		_pipeline->setDescriptorSetLayout(_descriptorSetLayout);
		_pipeline->setDescriptorSets(_descriptorSets);
		_pipeline->finalize(graphics);
	}

	void mvTexturedQuad::update(mvGraphics& graphics)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		mvTransforms transforms{};
		transforms.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		transforms.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		transforms.proj = glm::perspective(glm::radians(45.0f),
			graphics.getSwapChainExtent().width / (float)graphics.getSwapChainExtent().height,
			0.1f, 10.0f);
		transforms.proj[1][1] *= -1;

		auto index = graphics.getCurrentImageIndex();
		_uniformBuffers[index]->update(graphics, transforms);
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