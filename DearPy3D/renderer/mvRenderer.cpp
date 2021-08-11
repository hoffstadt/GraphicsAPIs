#include "mvRenderer.h"
#include "mvGraphics.h"
#include "mvDrawable.h"
#include "mvMaterial.h"
#include "mvCamera.h"

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

	void mvRenderer::renderDrawable(mvDrawable& drawable, mvMaterial& material)
	{
		drawable.bind();
		material.bind();

		_transforms.model = drawable.getTransform();
		_transforms.modelView = _camera * _transforms.model;
		_transforms.modelViewProjection = _projection * _transforms.modelView;

		vkCmdPushConstants(
			mvGraphics::GetContext().getSwapChain().getCurrentCommandBuffer(),
			material.getPipeline().getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mvRenderer::Transforms),&_transforms);

		mvGraphics::GetContext().getSwapChain().draw(drawable.getVertexCount());
	}

	void mvRenderer::setCamera(mvCamera& camera)
	{
		_camera = camera.getMatrix();
		_projection = camera.getProjection();
	}

}