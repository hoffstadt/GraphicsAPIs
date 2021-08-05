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

		mvImGuiManager(GLFWwindow* window, mvGraphics& graphics);
		~mvImGuiManager();

		void beginFrame(mvGraphics& graphics) const;
		void endFrame(mvGraphics& graphics) const;

	};

}