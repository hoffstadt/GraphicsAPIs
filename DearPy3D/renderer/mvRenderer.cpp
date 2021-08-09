#include "mvRenderer.h"
#include "mvGraphics.h"

namespace DearPy3D {

	void mvRenderer::beginFrame()
	{
		mvGraphics::GetContext().getSwapChain().beginFrame();
	}

	void mvRenderer::endFrame()
	{
		mvGraphics::GetContext().getSwapChain().endFrame();
		mvGraphics::GetContext().getSwapChain().present();
	}

}