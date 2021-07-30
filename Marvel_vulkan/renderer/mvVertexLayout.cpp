#include "mvVertexLayout.h"

namespace Marvel {

	mvVertexElement::mvVertexElement(ElementType type)
	{
		_type = type;
		switch (type)
		{

		case ElementType::Position2D:
			_format = VK_FORMAT_R32G32_SFLOAT;
			_itemCount = 2;
			_size = sizeof(float) * 2;
			break;

		case ElementType::Texture2D:
			_format = VK_FORMAT_R32G32_SFLOAT;
			_itemCount = 2;
			_size = sizeof(float) * 2;
			break;

		case ElementType::Color:
			_format = VK_FORMAT_R32G32B32_SFLOAT;
			_itemCount = 3;
			_size = sizeof(float) * 3;
			break;

		}
	}

	void mvVertexLayout::append(ElementType type)
	{
		for (const auto& element : _elements)
		{
			if (element._type == type)
				return;
		}
		_elements.emplace_back(type);
		_elements.back()._offset = _stride;
		_stride += _elements.back()._size;
		_size += _elements.back()._size;
	}

	std::vector<VkVertexInputBindingDescription> mvVertexLayout::getBindingDescriptions() const
	{
		std::vector<VkVertexInputBindingDescription> descriptions;

		for (const auto& element : _elements)
		{
			VkVertexInputBindingDescription description{};
			description.binding = 0;
			description.stride = _stride;
			description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			descriptions.push_back(description);
		}

		return descriptions;
	}

	std::vector<VkVertexInputAttributeDescription> mvVertexLayout::getAttributeDescriptions() const
	{
		std::vector<VkVertexInputAttributeDescription> descriptions;

		int index = 0;
		for (const auto& element : _elements)
		{
			VkVertexInputAttributeDescription description{};
			description.binding = 0;
			description.location = index++;
			description.format = element._format;
			description.offset = element._offset;

			descriptions.push_back(description);
		}

		return descriptions;
	}

}