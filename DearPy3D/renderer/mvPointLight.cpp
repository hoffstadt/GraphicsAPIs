#include "mvPointLight.h"
#include "mvGraphics.h"

namespace DearPy3D {


	mvPointLight::mvPointLight(glm::vec3 pos)
		:
		_mesh()
	{
		_info.viewLightPos = glm::vec4(pos, 1.0f);

		for (int i = 0; i < 3; i++)
			_buffer.push_back(std::make_unique<mvBuffer<PointLightInfo>>(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

		_mesh.setPosition(pos.x, pos.y, pos.z);
	}

	void mvPointLight::bind(glm::mat4 view)
	{
		glm::vec4 posCopy = _info.viewLightPos;

		glm::vec3 out = view * _info.viewLightPos;
		_info.viewLightPos.x = out.x;
		_info.viewLightPos.y = out.y;
		_info.viewLightPos.z = out.z;

		_buffer[mvGraphics::GetContext().getSwapChain().getCurrentImageIndex()]->update(_info);

		_info.viewLightPos = posCopy;
	}

	void mvPointLight::submit()
	{
		_mesh.bind();
		//_mesh.draw();
	}

	void mvPointLight::cleanup()
	{
		for (auto& item : _buffer)
			item->cleanup();
		_mesh.cleanup();
	}
}