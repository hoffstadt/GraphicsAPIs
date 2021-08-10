#pragma once

#include "mvMath.h"
#include "mvSolidSphere.h"

namespace DearPy3D {

	//-----------------------------------------------------------------------------
	// mvPointLight
	//-----------------------------------------------------------------------------
	class mvPointLight
	{

		struct PointLightInfo
		{

			glm::vec4 viewLightPos = { 0.0f, 0.0f, 0.0f, 1.0f };
			//-------------------------- ( 16 bytes )

			glm::vec3 diffuseColor = { 1.0f, 1.0f, 1.0f };
			float diffuseIntensity = 1.0f;
			//-------------------------- ( 16 bytes )

			float attConst = 1.0f;
			float attLin = 0.045f;
			float attQuad = 0.0075f;
			char _pad1[4];
			//-------------------------- ( 16 bytes )

			//-------------------------- ( 4*16 = 64 bytes )
		};

	public:

		mvPointLight(glm::vec3 pos = { 0.0f,0.0f,0.5f });

		void bind(glm::mat4 view);
		void submit();
		void cleanup();

	private:

		mvSolidSphere                          _mesh;
		std::vector<std::unique_ptr<mvBuffer<PointLightInfo>>> _buffer;
		PointLightInfo                  _info = {};

	};

}