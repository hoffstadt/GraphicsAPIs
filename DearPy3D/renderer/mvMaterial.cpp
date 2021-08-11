#include "mvMaterial.h"
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

		_descriptorSetLayout = std::make_shared<mvDescriptorSetLayout>();
		_descriptorSetLayout->append(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
		_descriptorSetLayout->finalize();

		for (int i = 0; i < 3; i++)
		{
			_descriptorSets.push_back(std::make_shared<mvDescriptorSet>());
			mvGraphics::GetContext().allocateDescriptorSet(&(*_descriptorSets.back()), *_descriptorSetLayout);
			std::vector<VkWriteDescriptorSet> descriptorWrites;
			descriptorWrites.resize(1);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = _texture->getImageView();
			imageInfo.sampler = _sampler->getSampler();

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = *_descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pImageInfo = &imageInfo;

			_descriptorSets[i]->update(descriptorWrites);
		}

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
			_descriptorSetLayout->cleanup();
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
		_descriptorSets[index]->bind(*_pipeline);
		_pipeline->bind();
	}

}