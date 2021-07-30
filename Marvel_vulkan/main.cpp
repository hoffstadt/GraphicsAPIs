#include <vector>
#include "mvWindow.h"
#include "mvDevice.h"
#include "mvGraphicsContext.h"

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
    // create and finalize a single pipeline
    //---------------------------------------------------------------------
    auto pipeline = std::make_shared<mvPipeline>();
    pipeline->setVertexLayout(vlayout);
    pipeline->setVertexShader(graphics, "../../Marvel_vulkan/shaders/vert.spv");
    pipeline->setFragmentShader(graphics, "../../Marvel_vulkan/shaders/frag.spv");
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