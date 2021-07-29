#include <vector>
#include "mvWindow.h"
#include "mvDevice.h"
#include "mvGraphicsContext.h"

using namespace Marvel;

int main() 
{

    auto window = mvWindow("Marvel Vulkan", 800, 600);
    auto device = mvDevice(window.getHandle());
    auto graphics = mvGraphicsContext(device);
    auto pipeline = std::make_shared<mvPipeline>(device);
    auto vertexShader = std::make_shared<mvShader>(device, "../../Marvel_vulkan/shaders/vert.spv");
    auto fragShader = std::make_shared<mvShader>(device, "../../Marvel_vulkan/shaders/frag.spv");

    auto vlayout = mvVertexLayout();
    vlayout.append(ElementType::Position2D);
    vlayout.append(ElementType::Color);

    pipeline->setVertexLayout(vlayout);
    pipeline->setVertexShader(vertexShader);
    pipeline->setFragmentShader(fragShader);
    device.createPipeline(*pipeline);
    graphics.setPipeline(pipeline);

    device.createCommandPool(graphics);

    auto indexBuffer = std::make_shared<mvIndexBuffer>(device, graphics, std::vector<uint16_t>{ 0u, 1u, 2u, 2u, 3u, 0u });

    auto vertexBuffer = std::make_shared<mvVertexBuffer>(device, graphics, vlayout, std::vector<float>{
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 1.0f, 1.0f, 1.0f
    });

    graphics.setIndexBuffer(indexBuffer);
    graphics.setVertexBuffer(vertexBuffer);
    
    device.createCommandBuffers(graphics);

    while (window.isRunning())
    {
        window.processEvents();

        device.present(graphics);
    }

}