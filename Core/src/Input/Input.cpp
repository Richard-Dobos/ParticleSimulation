#include "Input.h"

namespace Core::Input
{
	static bool isKeyPressed(KeyCode key)
	{
		return glfwGetKey(Core::Window::getGLFWCurrentContext(), (int)key);
	}

	static bool isMouseButtonPressed(KeyCode mouseButton)
	{
		return glfwGetMouseButton(Core::Window::getGLFWCurrentContext(), (int)mouseButton);
	}

	static double* getMousePosition()
	{
		double mousePos[2];
		glfwGetCursorPos(Core::Window::getGLFWCurrentContext(), &mousePos[0], &mousePos[1]);
		return mousePos;
	}

	static double getMouseX()
	{
		double* mousePos = getMousePosition();
		return mousePos[0];
	}

	static double getMouseY()
	{
		double* mousePos = getMousePosition();
		return mousePos[1];
	}
}