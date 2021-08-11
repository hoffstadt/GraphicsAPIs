#include "mvTransformUniform.h"
#include "mvGraphics.h"
#include "mvDrawable.h"

namespace DearPy3D {

	void mvTransformUniform::bind()
	{
		auto model = _parent->getTransform();
		auto modelView = mvGraphics::GetContext().getCamera() * _parent->getTransform();
		auto modelViewProj = mvGraphics::GetContext().getProjection() * mvGraphics::GetContext().getCamera() * _parent->getTransform();

		_transforms.model = model;
		_transforms.modelView = modelView;
		_transforms.modelViewProjection = modelViewProj;
	}

}