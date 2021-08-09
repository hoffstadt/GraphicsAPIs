#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvTexturedQuad.h"
#include "mvCube.h"
#include "mvCamera.h"
#include "mvTimer.h"

using namespace DearPy3D;

int main() 
{
    int width = 1000;
    int height = 1000;
    auto window = mvWindow("Dear Py3D", width, height);
    auto graphics = mvGraphics(window.getHandle());

    auto camera = mvCamera(graphics, width, height, glm::vec3{5.0f, 5.0f, -15.0f});

    auto quad1 = std::make_shared<mvTexturedQuad>(graphics, "../../Resources/brickwall.jpg");
    auto cube1 = std::make_shared<mvCube>(graphics, "../../Resources/brickwall.jpg");
    cube1->setPosition(10, 10, 10);

    //---------------------------------------------------------------------
    // main loop
    //---------------------------------------------------------------------
    mvTimer timer;

    while (window.isRunning())
    {
        const auto dt = timer.mark() * 1.0f;

        window.processEvents();

        if (window.isResized())
        {

            int newwidth = 0, newheight = 0;
            glfwGetFramebufferSize(window.getHandle(), &newwidth, &newheight);
            while (newwidth == 0 || newheight == 0)
            {
                glfwGetFramebufferSize(window.getHandle(), &newwidth, &newheight);
                glfwWaitEvents();
            }
            quad1->cleanup(graphics);
            cube1->cleanup(graphics);
            graphics.recreateSwapChain(newwidth, newheight);
            quad1.reset();
            cube1.reset();
            quad1 = std::make_shared<mvTexturedQuad>(graphics, "../../Resources/brickwall.jpg");
            cube1 = std::make_shared<mvCube>(graphics, "../../Resources/brickwall.jpg");
            cube1->setPosition(10, 10, 10);
            window.setResized(false);
            camera.setWidth(newwidth);
            camera.setHeight(newheight);
        }

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
        graphics.getSwapChain().beginFrame(graphics);

        //---------------------------------------------------------------------
        // record command buffers
        //---------------------------------------------------------------------
        
        graphics.getSwapChain().begin(graphics);

        camera.bind(graphics);
        quad1->bind(graphics);
        quad1->draw(graphics);

        cube1->bind(graphics);
        cube1->draw(graphics);

        graphics.getSwapChain().end(graphics);

        //---------------------------------------------------------------------
        // submit command buffers
        //---------------------------------------------------------------------
        graphics.getSwapChain().endFrame(graphics);

        //---------------------------------------------------------------------
        // present
        //---------------------------------------------------------------------
        graphics.getSwapChain().present(graphics);
    }

    cube1->cleanup(graphics);
    quad1->cleanup(graphics);
    graphics.cleanup();

}