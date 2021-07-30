#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Marvel {

	enum class ElementType
	{
		Position2D,
		Texture2D,
		Color
	};

	//-----------------------------------------------------------------------------
	// mvVertexElement
	//-----------------------------------------------------------------------------
	class mvVertexElement
	{

		friend class mvVertexLayout;

	public:

		mvVertexElement(ElementType type);

	private:

		int         _itemCount = 0;
		size_t      _size = 0;
		size_t      _offset = 0;
		VkFormat    _format = VK_FORMAT_UNDEFINED;
		ElementType _type;

	};

	//-----------------------------------------------------------------------------
	// mvVertexLayout
	//-----------------------------------------------------------------------------
	class mvVertexLayout
	{

	public:

		void                                           append(ElementType type);
		std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() const;
		std::vector<VkVertexInputBindingDescription>   getBindingDescriptions() const;

	private:

		size_t _stride = 0;
		size_t _size = 0;
		std::vector<mvVertexElement> _elements;

	};

}