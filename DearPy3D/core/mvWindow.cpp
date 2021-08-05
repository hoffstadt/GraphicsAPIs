#include "mvWindow.h"

namespace DearPy3D {

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		mvWindow* win = static_cast<mvWindow*>(glfwGetWindowUserPointer(window));

		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			if (win->cursorEnabled())
				win->disableCursor();

			else
				win->enableCursor();
		}
	}

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{

		mvWindow* win = static_cast<mvWindow*>(glfwGetWindowUserPointer(window));

		float xoffset = xpos - win->getLastX();
		float yoffset = ypos - win->getLastY();
		win->setDelta(xoffset, yoffset);
		win->setLast(xpos, ypos);
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) 
	{
		mvWindow* win = static_cast<mvWindow*>(glfwGetWindowUserPointer(window));

		win->setResized(true);
	}

	mvWindow::mvWindow(const char* name, int width, int height)
		:
		_width(width),
		_height(height),
		_handle(nullptr),
		_running(true)
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		_handle = glfwCreateWindow(width, height, name, nullptr, nullptr);
		glfwSetWindowUserPointer(_handle, this);
		glfwSetCursorPosCallback(_handle, mouse_callback);
		glfwSetKeyCallback(_handle, key_callback);
		glfwSetFramebufferSizeCallback(_handle, framebufferResizeCallback);

	}

	mvWindow::~mvWindow()
	{
		glfwTerminate();
	}

	void mvWindow::processEvents()
	{
		_deltaX = 0.0;
		_deltaY = 0.0;
		_running = !glfwWindowShouldClose(_handle);
		glfwPollEvents();
	}

	void mvWindow::enableCursor()
	{
		_cursorEnabled = true;
		glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void mvWindow::disableCursor()
	{
		_cursorEnabled = false;
		glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

}