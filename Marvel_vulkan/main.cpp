#include <vector>
#include "mvWindow.h"
#include "mvDevice.h"
#include "mvGraphicsContext.h"

using namespace Marvel;

int main() 
{

    auto window = mvWindow("Marvel Vulkan", 800, 600);
    auto graphics = mvGraphicsContext(window.getHandle());
    auto pipeline = std::make_shared<mvPipeline>();

    auto vlayout = mvVertexLayout();
    vlayout.append(ElementType::Position3D);
    vlayout.append(ElementType::Color);
    vlayout.append(ElementType::Texture2D);

    pipeline->setVertexLayout(vlayout);
    pipeline->setVertexShader(graphics, "../../Marvel_vulkan/shaders/vert.spv");
    pipeline->setFragmentShader(graphics, "../../Marvel_vulkan/shaders/frag.spv");
    pipeline->create(graphics);
    
    
    auto indexBuffer = std::make_shared<mvIndexBuffer>(graphics, 
        std::vector<uint16_t>{ 
        0u, 1u, 2u, 2u, 3u, 0u,
        4u, 5u, 6u, 6u, 7u, 4u
    });

    auto vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, vlayout, std::vector<float>{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
    });

    graphics.setPipeline(pipeline);
    graphics.setIndexBuffer(indexBuffer);
    graphics.setVertexBuffer(vertexBuffer);
    
    graphics.getDevice().createCommandBuffers(graphics, 0);
    graphics.getDevice().createCommandBuffers(graphics, 1);
    graphics.getDevice().createCommandBuffers(graphics, 2);

    //---------------------------------------------------------------------
    // main loop
    //---------------------------------------------------------------------
    while (window.isRunning())
    {
        window.processEvents();
        graphics.getDevice().present(graphics);
    }

    //---------------------------------------------------------------------
    // cleanup - crappy for now
    //---------------------------------------------------------------------
    graphics.getDevice().finish();
    vertexBuffer->finish(graphics);
    indexBuffer->finish(graphics);

}