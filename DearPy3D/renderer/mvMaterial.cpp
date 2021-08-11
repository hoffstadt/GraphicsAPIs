#include "mvMaterial.h"
#include <stdexcept>
#include "mvGraphics.h"
#include "mvTexture.h"
#include "mvSampler.h"

namespace DearPy3D {

	mvMaterial::mvMaterial()
	{
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
		bindings.resize(1);

		bindings[0].binding = 0u;
		bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		bindings[0].descriptorCount = 1;
		bindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings[0].pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(mvGraphics::GetContext().getLogicalDevice(), &layoutInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
			throw std::runtime_error("failed to create descriptor set layout!");

		//-----------------------------------------------------------------------------
		// allocate descriptor sets
		//-----------------------------------------------------------------------------
		_descriptorSets.resize(3);
		std::vector<VkDescriptorSetLayout> layouts(3, _descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = mvGraphics::GetContext().getDescriptorPool();
		allocInfo.descriptorSetCount = 3;
		allocInfo.pSetLayouts = layouts.data();

		if (vkAllocateDescriptorSets(mvGraphics::GetContext().getLogicalDevice(), &allocInfo, _descriptorSets.data()) != VK_SUCCESS)
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
			descriptorWrites[i].dstSet = _descriptorSets[i];
			descriptorWrites[i].dstBinding = 0;
			descriptorWrites[i].dstArrayElement = 0;
			descriptorWrites[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[i].descriptorCount = 1;
			descriptorWrites[i].pImageInfo = &imageInfo;
		}

		vkUpdateDescriptorSets(mvGraphics::GetContext().getLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		_pipeline = std::make_shared<mvPipeline>();
		_pipeline->setVertexLayout(vlayout);
		_pipeline->setVertexShader("../../DearPy3D/shaders/vs_shader.vert.spv");
		_pipeline->setFragmentShader("../../DearPy3D/shaders/ps_shader.frag.spv");
		_pipeline->setDescriptorSetLayout(_descriptorSetLayout);
		_pipeline->setDescriptorSets(_descriptorSets);
		_pipeline->finalize();

		_deletionQueue.pushDeletor([=]() {
			_sampler->cleanup();
			_texture->cleanup();
			vkDestroyDescriptorSetLayout(mvGraphics::GetContext().getLogicalDevice(), _descriptorSetLayout, nullptr);
			});
	}

	void mvMaterial::cleanup()
	{
		vkDeviceWaitIdle(mvGraphics::GetContext().getLogicalDevice());
		_deletionQueue.flush();
	}

	void mvMaterial::bind()
	{
		auto index = mvGraphics::GetContext().getSwapChain().getCurrentImageIndex();

		vkCmdBindDescriptorSets(mvGraphics::GetContext().getSwapChain().getCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, 
			_pipeline->getLayout(), 0, 1, _descriptorSets.data(), 0, nullptr);

		_pipeline->bind();
	}

}