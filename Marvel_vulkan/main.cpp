#include "mvWindow.h"
#include "mvGraphics.h"

using namespace Marvel;

int main() 
{

    auto window = mvWindow("Marvel Vulkan", 800, 600);
    auto graphics = mvGraphics(window.getHandle());

    while (window.isRunning())
    {
        window.processEvents();

        graphics.present();
    }

}