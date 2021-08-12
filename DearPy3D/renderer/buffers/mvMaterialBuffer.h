#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>
#include "mvMath.h"

namespace DearPy3D {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvDrawable;
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvTransformUniform
	//-----------------------------------------------------------------------------
	class mvMaterialBuffer
	{
		friend class mvMaterial;

	public:

		struct mvMaterialData
		{
			glm::vec4 materialColor = { 0.45f, 0.45f, 0.85f, 1.0f };
			//-------------------------- ( 16 bytes )

			glm::vec3 specularColor = { 0.18f, 0.18f, 0.18f };
			float specularGloss = 8.0f;
			//-------------------------- ( 16 bytes )

			float normalMapWeight = 1.0f;
			int useTextureMap = false;
			int useNormalMap = false;
			int useSpecularMap = false;
			//-------------------------- ( 16 bytes )

			int useGlossAlpha = false;
			int hasAlpha = false;
			char _pad1[8];
			//-------------------------- ( 16 bytes )

			char _pad2[192];
			//-------------------------- ( 4 * 16 = 256 bytes )
		};


	public:

		mvMaterialBuffer();

		void bind(mvMaterialData data, uint32_t index);
		void cleanup();

	private:

		std::vector<VkBuffer>       _buffer;
		std::vector<VkDeviceMemory> _bufferMemory;

	};

}