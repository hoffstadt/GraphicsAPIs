#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvTexturedQuad.h"
#include "mvCamera.h"
#include "mvTimer.h"

using namespace DearPy3D;

int main() 
{

    auto window = mvWindow("Marvel Vulkan", 800, 600);
    auto graphics = mvGraphics(window.getHandle());

    auto camera = mvCamera(graphics, 800, 600, glm::vec3{5.0f, 5.0f, -15.0f});

    auto quad1 = std::make_shared<mvTexturedQuad>(graphics, "../../Resources/brickwall.jpg");

    //---------------------------------------------------------------------
    // main loop
    //---------------------------------------------------------------------
    mvTimer timer;

    while (window.isRunning())
    {
        const auto dt = timer.mark() * 1.0f;

        window.processEvents();

        if (glfwGetKey(window.getHandle(), GLFW_KEY_W) == GLFW_PRESS) camera.translate(0.0f, 0.0f, dt);
        if (glfwGetKey(window.getHandle(), GLFW_KEY_S) == GLFW_PRESS) camera.translate(0.0f, 0.0f, -dt);
        if (glfwGetKey(window.getHandle(), GLFW_KEY_A) == GLFW_PRESS) camera.translate(-dt, 0.0f, 0.0f);
        if (glfwGetKey(window.getHandle(), GLFW_KEY_D) == GLFW_PRESS)camera.translate(dt, 0.0f, 0.0f);
        if (glfwGetKey(window.getHandle(), GLFW_KEY_R) == GLFW_PRESS)camera.translate(0.0f, dt, 0.0f);
        if (glfwGetKey(window.getHandle(), GLFW_KEY_F) == GLFW_PRESS)camera.translate(0.0f, -dt, 0.0f);

        if (!window.cursorEnabled())
            camera.rotate(window.getDeltaX(), window.getDeltaY());
        
        //---------------------------------------------------------------------
        // wait for fences and acquire next image
        //---------------------------------------------------------------------
        graphics.beginFrame();

        //---------------------------------------------------------------------
        // record command buffers
        //---------------------------------------------------------------------
        
        graphics.begin();
        camera.bind(graphics);
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