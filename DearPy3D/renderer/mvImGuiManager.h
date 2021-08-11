#pragma once

#include <imgui.h>

//---------------------------------------------------------------------
// forward declarations
//---------------------------------------------------------------------
struct GLFWwindow;

namespace DearPy3D {

	//---------------------------------------------------------------------
	// forward declarations
	//---------------------------------------------------------------------
	class mvGraphics;

	//---------------------------------------------------------------------
	// mvImGuiManager
	//---------------------------------------------------------------------
	class mvImGuiManager
	{

	public:

		mvImGuiManager(GLFWwindow* window);
		
		void cleanup();
		void resize();
		void beginFrame() const;
		void endFrame() const;

	};

}