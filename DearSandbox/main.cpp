#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvTexturedQuad.h"
#include "mvCube.h"
#include "mvCamera.h"
#include "mvTimer.h"
#include "mvRenderer.h"
#include "mvPointLight.h"
#include "mvMaterial.h"
#include "mvImGuiManager.h"

using namespace DearPy3D;

int g_width = 500;
int g_height = 500;

int main() 
{

    auto window = mvWindow("Dear Py3D", g_width, g_height);
    mvGraphics::Init(window.getHandle());
    auto renderer = mvRenderer();
    auto imgui = mvImGuiManager(window.getHandle());
    auto camera = mvCamera(g_width, g_height, glm::vec3{5.0f, 5.0f, -15.0f});

    auto quad1 = std::make_shared<mvTexturedQuad>("../../Resources/brickwall.jpg");
    auto cube1 = std::make_shared<mvCube>("../../Resources/brickwall.jpg");
    cube1->setPosition(10, 10, 10);

    auto material = std::make_shared<mvMaterial>();

    //---------------------------------------------------------------------
    // main loop
    //---------------------------------------------------------------------
    mvTimer timer;

    while (window.isRunning())
    {
        const auto dt = timer.mark() * 1.0f;

        window.processEvents();

        //---------------------------------------------------------------------
        // handle window resizing
        //---------------------------------------------------------------------
        if (window.isResized())
        {

            int newwidth = 0, newheight = 0;
            glfwGetFramebufferSize(window.getHandle(), &newwidth, &newheight);
            while (newwidth == 0 || newheight == 0)
            {
                glfwGetFramebufferSize(window.getHandle(), &newwidth, &newheight);
                glfwWaitEvents();
            }

            camera.updateProjection(newwidth, newheight);

            // cleanup
            material->cleanup();
            cube1->cleanup();
            quad1->cleanup();
            mvGraphics::GetContext().recreateSwapChain(newwidth, newheight);
            imgui.resize();

            // recreation
            quad1 = std::make_shared<mvTexturedQuad>("../../Resources/brickwall.jpg");
            cube1 = std::make_shared<mvCube>("../../Resources/brickwall.jpg");
            cube1->setPosition(10, 10, 10);
            material = std::make_shared<mvMaterial>();
        }

        //---------------------------------------------------------------------
        // input handling
        //---------------------------------------------------------------------
        if (glfwGetKey(window.getHandle(), GLFW_KEY_W) == GLFW_PRESS) camera.translate(0.0f, 0.0f, dt);
        if (glfwGetKey(window.getHandle(), GLFW_KEY_S) == GLFW_PRESS) camera.translate(0.0f, 0.0f, -dt);
        if (glfwGetKey(window.getHandle(), GLFW_KEY_A) == GLFW_PRESS) camera.translate(-dt, 0.0f, 0.0f);
        if (glfwGetKey(window.getHandle(), GLFW_KEY_D) == GLFW_PRESS)camera.translate(dt, 0.0f, 0.0f);
        if (glfwGetKey(window.getHandle(), GLFW_KEY_R) == GLFW_PRESS)camera.translate(0.0f, dt, 0.0f);
        if (glfwGetKey(window.getHandle(), GLFW_KEY_F) == GLFW_PRESS)camera.translate(0.0f, -dt, 0.0f);
        if (!window.cursorEnabled()) camera.rotate(window.getDeltaX(), window.getDeltaY());
        
        //---------------------------------------------------------------------
        // wait for fences and acquire next image
        //---------------------------------------------------------------------
        renderer.beginFrame();

        //---------------------------------------------------------------------
        // main pass
        //---------------------------------------------------------------------
        
        auto currentCommandBuffer = mvGraphics::GetContext().getSwapChain().getCurrentCommandBuffer();

        renderer.beginPass(currentCommandBuffer, mvGraphics::GetContext().getSwapChain().getRenderPass());
        imgui.beginFrame();

        renderer.setCamera(camera);

        renderer.renderDrawable(*cube1, material->getPipeline());
        renderer.renderDrawable(*quad1, material->getPipeline());

        imgui.endFrame();
        renderer.endPass(currentCommandBuffer);

        //---------------------------------------------------------------------
        // submit command buffers & present
        //---------------------------------------------------------------------
        renderer.endFrame();
    }

    material->cleanup();
    cube1->cleanup();
    quad1->cleanup();
    imgui.cleanup();
    mvGraphics::Shutdown();
}