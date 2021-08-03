#include <vector>
#include <chrono>
#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvGraphics.h"
#include "mvDescriptorSet.h"
#include "mvDescriptorPool.h"
#include "mvBuffer.h"
#include "mvCommandBuffer.h"
#include "mvTexture.h"
#include "mvSampler.h"

using namespace Marvel;

int main() 
{

    auto window = mvWindow("Marvel Vulkan", 800, 600);
    auto graphics = mvGraphics(window.getHandle());

    struct mvTransforms
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    std::vector<std::shared_ptr<mvBuffer<mvTransforms>>> uniformBuffers;
    for (int i = 0; i < 3; i++)
        uniformBuffers.push_back(std::make_shared<mvBuffer<mvTransforms>>(graphics,VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

    //---------------------------------------------------------------------
    // create vertex buffer, vertex layout, and index buffer
    //---------------------------------------------------------------------

    auto vlayout = mvVertexLayout();
    vlayout.append(ElementType::Position3D);
    vlayout.append(ElementType::Color);
    vlayout.append(ElementType::Texture2D);

    auto vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, std::vector<float>{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    });

    auto indexBuffer = std::make_shared<mvIndexBuffer>(graphics, std::vector<uint16_t>{ 
        0u, 1u, 2u, 2u, 3u, 0u,
        4u, 5u, 6u, 6u, 7u, 4u
    });

    //---------------------------------------------------------------------
    // create texture and sampler
    //---------------------------------------------------------------------
    auto texture = std::make_shared<mvTexture>(graphics, "../../Resources/brickwall.jpg");
    auto sampler = std::make_shared<mvSampler>(graphics);

    //---------------------------------------------------------------------
    // create descriptor set layout
    //---------------------------------------------------------------------
    auto descriptorSetLayout = std::make_shared<mvDescriptorSetLayout>();
    descriptorSetLayout->append(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    descriptorSetLayout->append(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
    descriptorSetLayout->finalize(graphics);

    //---------------------------------------------------------------------
    // create descriptor pool
    //---------------------------------------------------------------------
    auto descriptorPool = std::make_shared<mvDescriptorPool>(graphics);

    //---------------------------------------------------------------------
    // create and update descriptor sets
    //---------------------------------------------------------------------
    std::vector<std::shared_ptr<mvDescriptorSet>> descriptorSets;
    for (int i = 0; i < 3; i++)
    {
        descriptorSets.push_back(std::make_shared<mvDescriptorSet>());
        descriptorPool->allocateDescriptorSet(graphics, &(*descriptorSets.back()), *descriptorSetLayout);
        descriptorSets.back()->update(graphics, *uniformBuffers[i], texture->getImageView(), sampler->getSampler());
    }

    //---------------------------------------------------------------------
    // create and finalize a single pipeline
    //---------------------------------------------------------------------
    auto pipeline = std::make_shared<mvPipeline>();
    pipeline->setVertexLayout(vlayout);
    pipeline->setVertexShader(graphics, "../../Marvel_vulkan/shaders/vert.spv");
    pipeline->setFragmentShader(graphics, "../../Marvel_vulkan/shaders/frag.spv");
    pipeline->setDescriptorSetLayout(descriptorSetLayout);
    pipeline->setDescriptorSets(descriptorSets);
    pipeline->finalize(graphics);

    //---------------------------------------------------------------------
    // create command pool and buffers
    //---------------------------------------------------------------------
    std::vector<std::shared_ptr<mvCommandBuffer>> commandBuffers;
    for (int i = 0; i < 3; i++)
    {
        commandBuffers.push_back(std::make_shared<mvCommandBuffer>());
        graphics.allocateCommandBuffer(commandBuffers.back().get());
    }

    //---------------------------------------------------------------------
    // main loop
    //---------------------------------------------------------------------
    while (window.isRunning())
    {
        window.processEvents();
        
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

        //---------------------------------------------------------------------
        // wait for fences and acquire next image
        //---------------------------------------------------------------------
        graphics.begin();
        auto index = graphics.getCurrentImageIndex();

        //---------------------------------------------------------------------
        // update uniform buffers
        //---------------------------------------------------------------------
        uniformBuffers[index]->update(graphics, transforms);
        
        //---------------------------------------------------------------------
        // record command buffers
        //---------------------------------------------------------------------
        auto& commandBuffer = commandBuffers[index];
        commandBuffer->beginRecording(graphics);
        descriptorSets[index]->bind(*commandBuffer, *pipeline);
        pipeline->bind(*commandBuffer);
        vertexBuffer->bind(*commandBuffer);
        indexBuffer->bind(*commandBuffer);
        commandBuffer->draw(indexBuffer->getVertexCount());
        commandBuffer->endRecording();

        //---------------------------------------------------------------------
        // submit command buffers
        //---------------------------------------------------------------------
        graphics.submit(*commandBuffers[index]);

        //---------------------------------------------------------------------
        // present
        //---------------------------------------------------------------------
        graphics.present();
    }

    //---------------------------------------------------------------------
    // cleanup - crappy for now, need to add a deletion queue
    //---------------------------------------------------------------------

}