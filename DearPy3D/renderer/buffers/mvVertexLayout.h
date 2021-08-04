#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace DearPy3D {

	enum class ElementType
	{
		Position2D,
		Position3D,
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

		size_t                       _stride = 0;
		size_t                       _size = 0;
		std::vector<mvVertexElement> _elements;

	};

}