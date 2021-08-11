#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvTexturedQuad.h"
#include "mvCube.h"
#include "mvCamera.h"
#include "mvTimer.h"
#include "mvRenderer.h"
#include "mvPointLight.h"
#include "mvMaterial.h"

using namespace DearPy3D;

int main() 
{
    int width = 1000;
    int height = 1000;
    auto window = mvWindow("Dear Py3D", width, height);
    mvGraphics::Init(window.getHandle());
    auto renderer = mvRenderer();

    //auto pointlight = mvPointLight(glm::vec3{ 3.0f, 3.0f, 0.0f });

    auto camera = mvCamera(width, height, glm::vec3{5.0f, 5.0f, -15.0f});

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

        if (window.isResized())
        {

            int newwidth = 0, newheight = 0;
            glfwGetFramebufferSize(window.getHandle(), &newwidth, &newheight);
            while (newwidth == 0 || newheight == 0)
            {
                glfwGetFramebufferSize(window.getHandle(), &newwidth, &newheight);
                glfwWaitEvents();
            }

            material->cleanup();
            cube1->cleanup();
            quad1->cleanup();
            mvGraphics::GetContext().recreateSwapChain(newwidth, newheight);
            quad1.reset();
            cube1.reset();
            material.reset();
            quad1 = std::make_shared<mvTexturedQuad>("../../Resources/brickwall.jpg");
            cube1 = std::make_shared<mvCube>("../../Resources/brickwall.jpg");
            cube1->setPosition(10, 10, 10);

            material = std::make_shared<mvMaterial>();
            window.setResized(false);
            camera.updateProjection(newwidth, newheight);
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
        renderer.beginFrame();

        //---------------------------------------------------------------------
        // record command buffers
        //---------------------------------------------------------------------
        
        mvGraphics::GetContext().getSwapChain().begin();

        renderer.setCamera(camera);
        //pointlight.bind(camera.getMatrix());

        renderer.renderDrawable(*cube1, *material);
        renderer.renderDrawable(*quad1, *material);

        //pointlight.submit();

        mvGraphics::GetContext().getSwapChain().end();

        //---------------------------------------------------------------------
        // submit command buffers & present
        //---------------------------------------------------------------------
        renderer.endFrame();

    }

    material->cleanup();
    cube1->cleanup();
    quad1->cleanup();
    //pointlight.cleanup();
    mvGraphics::Shutdown();

}