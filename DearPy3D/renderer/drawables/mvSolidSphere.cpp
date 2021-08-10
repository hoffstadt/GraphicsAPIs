#include "mvSolidSphere.h"
#include "mvGraphics.h"
#include "mvTexture.h"
#include "mvSampler.h"
#include "Sphere.h"
#include <chrono>

namespace DearPy3D {

	mvSolidSphere::mvSolidSphere()
	{

		_transformBuffer = std::make_shared<mvTransformUniform>();
		_transformBuffer->_parent = this;

		auto vlayout = mvVertexLayout();
		vlayout.append(ElementType::Position3D);
		vlayout.append(ElementType::Normal);
		vlayout.append(ElementType::Tangent);
		vlayout.append(ElementType::Bitangent);
		vlayout.append(ElementType::Texture2D);

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
		_sampler = std::make_shared<mvSampler>();
		_texture = std::make_shared<mvTexture>("../../Resources/brickwall.jpg");

		_descriptorSetLayout = std::make_shared<mvDescriptorSetLayout>();
		_descriptorSetLayout->append(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
		_descriptorSetLayout->append(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
		//_descriptorSetLayout->append(2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
		_descriptorSetLayout->finalize();

		for (int i = 0; i < 3; i++)
		{
			_descriptorSets.push_back(std::make_shared<mvDescriptorSet>());
			mvGraphics::GetContext().allocateDescriptorSet(&(*_descriptorSets.back()), *_descriptorSetLayout);
			std::vector<VkWriteDescriptorSet> descriptorWrites;
			descriptorWrites.resize(2);

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = _transformBuffer->_buf[i]->getBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(mvTransformUniform::Transforms);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = _texture->getImageView();
			imageInfo.sampler = _sampler->getSampler();

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = *_descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = *_descriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			_descriptorSets[i]->update(descriptorWrites);
		}

		_pipeline = std::make_shared<mvPipeline>();
		_pipeline->setVertexLayout(vlayout);
		_pipeline->setVertexShader("../../DearPy3D/shaders/vert.spv");
		_pipeline->setFragmentShader("../../DearPy3D/shaders/frag_solid.spv");
		_pipeline->setDescriptorSetLayout(_descriptorSetLayout);
		_pipeline->setDescriptorSets(_descriptorSets);
		_pipeline->finalize();

		_deletionQueue.pushDeletor([=]() {
			_sampler->cleanup();
			_texture->cleanup();
			_descriptorSetLayout->cleanup();
			_indexBuffer->cleanup();
			_vertexBuffer->cleanup();
			_transformBuffer->cleanup();
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

	void mvSolidSphere::bind() const
	{
		auto index = mvGraphics::GetContext().getSwapChain().getCurrentImageIndex();
		_transformBuffer->bind();
		_descriptorSets[index]->bind(*_pipeline);

		std::vector<VkWriteDescriptorSet> descriptorWrites;
		descriptorWrites.resize(2);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = _transformBuffer->_buf[index]->getBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(mvTransformUniform::Transforms);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = _texture->getImageView();
		imageInfo.sampler = _sampler->getSampler();

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = *_descriptorSets[index];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = *_descriptorSets[index];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		_descriptorSets[index]->update(descriptorWrites);

		_descriptorSets[index]->update(descriptorWrites);
		_pipeline->bind();
		_indexBuffer->bind();
		_vertexBuffer->bind();
	}

}