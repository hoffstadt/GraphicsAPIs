#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace DearPy3D {

	class mvWindow
	{

	public:

		mvWindow(const char* name, int width, int height);
		~mvWindow();

		void processEvents();

		GLFWwindow* getHandle() const { return _handle; }
		void enableCursor();
		void disableCursor();
		bool cursorEnabled() const { return _cursorEnabled; }
		bool isRunning() const { return _running; }
		void setDelta(double x, double y) { _deltaX = x; _deltaY = y; }
		void setLast(double x, double y) { _lastX = x; _lastY = y; }
		double getDeltaX() { return _deltaX; }
		double getDeltaY() { return _deltaY; }
		double getLastX() { return _lastX; }
		double getLastY() { return _lastY; }
		bool isResized() { bool orig = _resized; _resized = false; return orig; }
		void setResized(bool value) { _resized = value; }

	private:

		int         _width;
		int         _height;
		GLFWwindow* _handle;
		bool        _running;
		bool        _cursorEnabled;
		double      _lastX = 0.0;
		double      _lastY = 0.0;
		double      _deltaX = 0.0;
		double      _deltaY = 0.0;
		bool        _resized = false;

	};

}