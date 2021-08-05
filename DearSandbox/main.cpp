#include "mvWindow.h"
#include "mvGraphics.h"
#include "mvTexturedQuad.h"
#include "mvCamera.h"
#include "mvTimer.h"
#include "mvImGuiManager.h"

using namespace DearPy3D;

int main() 
{
    int width = 1000;
    int height = 1000;
    auto window = mvWindow("Dear Py3D", width, height);
    auto graphics = mvGraphics(window.getHandle());
    auto imguiManager = mvImGuiManager(window.getHandle(), graphics);

    auto camera = mvCamera(graphics, width, height, glm::vec3{5.0f, 5.0f, -15.0f});

    auto quad1 = std::make_shared<mvTexturedQuad>(graphics, "../../Resources/brickwall.jpg");

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
            
            graphics.recreateSwapChain(window.getHandle());
            quad1.reset();
            quad1 = std::make_shared<mvTexturedQuad>(graphics, "../../Resources/brickwall.jpg");
            window.setResized(false);
            imguiManager.resize(graphics);
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
        graphics.beginFrame();

        //---------------------------------------------------------------------
        // record command buffers
        //---------------------------------------------------------------------
        
        graphics.begin();

        camera.bind(graphics);
        quad1->bind(graphics);
        quad1->draw(graphics);

        imguiManager.beginFrame(graphics);
        ImGui::ShowDemoWindow();
        imguiManager.endFrame(graphics);

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

}