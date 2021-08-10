#pragma once

#include "mvMath.h"

namespace DearPy3D {

	class mvGraphics;

	class mvScene
	{

		struct GlobalSettings
		{

			glm::vec3 ambientColor = { 0.05f, 0.05f, 0.05f };
			char _pad0[4];
			//-------------------------- ( 16 bytes )

			glm::vec3 camPos = { 0.0f, 0.0f, 0.0f };
			char _pad1[4];
			//-------------------------- ( 16 bytes )

			//-------------------------- ( 2*16 = 32 bytes )
		};

	};

}