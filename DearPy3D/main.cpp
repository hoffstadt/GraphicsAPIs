#include <vector>
#include <chrono>
#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvGraphics.h"
#include "mvDescriptorSet.h"
#include "mvBuffer.h"
#include "mvTexture.h"
#include "mvSampler.h"

#include "mvTexturedQuad.h"

using namespace DearPy3D;

int main() 
{

    auto window = mvWindow("Marvel Vulkan", 800, 600);
    auto graphics = mvGraphics(window.getHandle());

    auto quad1 = std::make_shared<mvTexturedQuad>(graphics, "../../Resources/brickwall.jpg");

    //---------------------------------------------------------------------
    // main loop
    //---------------------------------------------------------------------
    while (window.isRunning())
    {
        window.processEvents();
        
        //---------------------------------------------------------------------
        // wait for fences and acquire next image
        //---------------------------------------------------------------------
        graphics.beginFrame();

        //---------------------------------------------------------------------
        // record command buffers
        //---------------------------------------------------------------------
        
        graphics.begin();
        quad1->update(graphics);
        quad1->bind(graphics);
        quad1->draw(graphics);
        graphics.end();

        //---------------------------------------------------------------------
        // submit command buffers
        //---------------------------------------------------------------------
        graphics.endFrame();

        //---------------------------------------------------------------------
        // present
        //---------------------------------------------------------------------
        graphics.present();
    }

    //---------------------------------------------------------------------
    // cleanup - crappy for now, need to add a deletion queue
    //---------------------------------------------------------------------

}