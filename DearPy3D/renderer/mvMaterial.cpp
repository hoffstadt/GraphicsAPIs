#include "mvMaterial.h"
#include <stdexcept>
#include "mvGraphics.h"
#include "mvTexture.h"
#include "mvSampler.h"

namespace DearPy3D {

	mvMaterial::mvMaterial()
	{

		_materialBuffer = std::make_shared<mvMaterialBuffer>();

		auto vlayout = mvVertexLayout();
		vlayout.append(ElementType::Position3D);
		vlayout.append(ElementType::Normal);
		vlayout.append(ElementType::Tangent);
		vlayout.append(ElementType::Bitangent);
		vlayout.append(ElementType::Texture2D);

		_sampler = std::make_shared<mvSampler>();
		_texture = std::make_shared<mvTexture>("../../Resources/brickwall.jpg");

		//-----------------------------------------------------------------------------
		// create descriptor set layout
		//-----------------------------------------------------------------------------
		std::vector<VkDescriptorSetLayoutBinding> bindings;
		bindings.resize(2);

		bindings[0].binding = 0u;
		bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		bindings[0].descriptorCount = 1;
		bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings[0].pImmutableSamplers = nullptr;

		bindings[1].binding = 1u;
		bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		bindings[1].descriptorCount = 1;
		bindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings[1].pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
		descriptorSetLayouts.resize(1);

		std::vector<VkDescriptorSet> descriptorSets;

		if (vkCreateDescriptorSetLayout(mvGraphics::GetContext().getLogicalDevice(), &layoutInfo, nullptr, &descriptorSetLayouts.back()) != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor set layout!");

		//-----------------------------------------------------------------------------
		// allocate descriptor sets
		//-----------------------------------------------------------------------------
		descriptorSets.resize(3);
		std::vector<VkDescriptorSetLayout> layouts(3, descriptorSetLayouts[0]);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = mvGraphics::GetContext().getDescriptorPool();
		allocInfo.descriptorSetCount = 3;
		allocInfo.pSetLayouts = layouts.data();

		if (vkAllocateDescriptorSets(mvGraphics::GetContext().getLogicalDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS)
			throw std::runtime_error("failed to allocate descriptor sets!");

		//-----------------------------------------------------------------------------
		// update descriptor sets
		//-----------------------------------------------------------------------------
		std::vector<VkWriteDescriptorSet> descriptorWrites;
		descriptorWrites.resize(3);
		for (int i = 0; i < 3; i++)
		{
			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = _texture->getImageView();
			imageInfo.sampler = _sampler->getSampler();

			descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[i].dstSet = descriptorSets[i];
			descriptorWrites[i].dstBinding = 0;
			descriptorWrites[i].dstArrayElement = 0;
			descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[i].descriptorCount = 1;
			descriptorWrites[i].pImageInfo = &imageInfo;

			VkDescriptorBufferInfo materialInfo;
			materialInfo.buffer = _materialBuffer->_buffer[i];
			materialInfo.offset = 0;
			materialInfo.range = sizeof(mvMaterialBuffer::mvMaterialData);

			descriptorWrites[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[i].dstSet = descriptorSets[i];
			descriptorWrites[i].dstBinding = 1;
			descriptorWrites[i].dstArrayElement = 0;
			descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			descriptorWrites[i].descriptorCount = 1;
			descriptorWrites[i].pBufferInfo = &materialInfo;
		}

		vkUpdateDescriptorSets(mvGraphics::GetContext().getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		_pipeline = std::make_shared<mvPipeline>();
		_pipeline->setVertexLayout(vlayout);
		_pipeline->setVertexShader("../../DearPy3D/shaders/vs_shader.vert.spv");
		_pipeline->setFragmentShader("../../DearPy3D/shaders/ps_shader.frag.spv");
		_pipeline->setDescriptorSetLayouts(descriptorSetLayouts);
		_pipeline->setDescriptorSets(descriptorSets);
		_pipeline->finalize();

		_deletionQueue.pushDeletor([=]() {
			_sampler->cleanup();
			_texture->cleanup();
			_materialBuffer->cleanup();
			});
	}

	void mvMaterial::bind(uint32_t index, mvMaterialBuffer::mvMaterialData data)
	{
		_materialBuffer->bind(data, index);
	}

	void mvMaterial::cleanup()
	{
		vkDeviceWaitIdle(mvGraphics::GetContext().getLogicalDevice());
		_deletionQueue.flush();
	}

}