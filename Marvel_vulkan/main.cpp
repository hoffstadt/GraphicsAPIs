#include <vector>
#include "mvWindow.h"
#include "mvDevice.h"
#include "mvGraphicsContext.h"
#include "mvDescriptorSet.h"

using namespace Marvel;

int main() 
{

    auto window = mvWindow("Marvel Vulkan", 800, 600);
    auto graphics = mvGraphicsContext(window.getHandle());

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
    // create descriptor set layout
    //---------------------------------------------------------------------
    auto descriptorSetLayout = std::make_shared<mvDescriptorSetLayout>();
    descriptorSetLayout->append(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    descriptorSetLayout->append(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
    descriptorSetLayout->finalize(graphics);

    //---------------------------------------------------------------------
    // create descriptor pool
    //---------------------------------------------------------------------
    auto descriptorPool = std::make_shared<mvDescriptorPool>();
    descriptorPool->finalize(graphics);

    //---------------------------------------------------------------------
    // create and update descriptor sets
    //---------------------------------------------------------------------
    auto descriptorSet = descriptorPool->allocateDescriptorSets(graphics, *descriptorSetLayout);
    descriptorSet->update(graphics);

    //---------------------------------------------------------------------
    // create and finalize a single pipeline
    //---------------------------------------------------------------------
    auto pipeline = std::make_shared<mvPipeline>();
    pipeline->setVertexLayout(vlayout);
    pipeline->setVertexShader(graphics, "../../Marvel_vulkan/shaders/vert.spv");
    pipeline->setFragmentShader(graphics, "../../Marvel_vulkan/shaders/frag.spv");
    pipeline->setDescriptorSetLayout(descriptorSetLayout);
    pipeline->setDescriptorSet(descriptorSet);
    pipeline->finalize(graphics);
    
    //---------------------------------------------------------------------
    // record command buffers
    //---------------------------------------------------------------------
    for (int i = 0; i < 3; i++)
    {
        graphics.beginRecording(i);
        pipeline->bind(graphics);
        vertexBuffer->bind(graphics);
        indexBuffer->bind(graphics);
        graphics.draw(indexBuffer->getVertexCount());
        graphics.endRecording();
    }

    //---------------------------------------------------------------------
    // main loop
    //---------------------------------------------------------------------
    while (window.isRunning())
    {
        window.processEvents();
        graphics.present();
    }

    //---------------------------------------------------------------------
    // cleanup - crappy for now, need to add a deletion queue
    //---------------------------------------------------------------------
    graphics.getDevice().finish();
    vertexBuffer->finish(graphics);
    indexBuffer->finish(graphics);
    pipeline->finish(graphics);

}