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
    auto vertexShader = std::make_shared<mvShader>(graphics, "../../Marvel_vulkan/shaders/vert.spv");
    auto fragShader = std::make_shared<mvShader>(graphics, "../../Marvel_vulkan/shaders/frag.spv");

    auto vlayout = mvVertexLayout();
    vlayout.append(ElementType::Position2D);
    vlayout.append(ElementType::Color);

    pipeline->setVertexLayout(vlayout);
    pipeline->setVertexShader(vertexShader);
    pipeline->setFragmentShader(fragShader);
    pipeline->create(graphics);
    graphics.setPipeline(pipeline);

    graphics.getDevice().createCommandPool(graphics);

    auto indexBuffer = std::make_shared<mvIndexBuffer>(graphics, std::vector<uint16_t>{ 0u, 1u, 2u, 2u, 3u, 0u });

    auto vertexBuffer = std::make_shared<mvVertexBuffer>(graphics, vlayout, std::vector<float>{
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 1.0f, 1.0f, 1.0f
    });

    graphics.setIndexBuffer(indexBuffer);
    graphics.setVertexBuffer(vertexBuffer);
    
    graphics.getDevice().createCommandBuffers(graphics);

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
    vertexShader->finish(graphics);
    fragShader->finish(graphics);
    graphics.getDevice().finish();
    vertexBuffer->finish(graphics);
    indexBuffer->finish(graphics);

}