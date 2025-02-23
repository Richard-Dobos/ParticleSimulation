#include "Input.h"
#include "Window/Window.h"

#include "glfw3.h"

namespace Core::Input
{
	bool isKeyPressed(KeyCode key)
	{
		return glfwGetKey(Core::Window::getGLFWCurrentContext(), (int)key);
	}

	bool isMouseButtonPressed(KeyCode mouseButton)
	{
		return glfwGetMouseButton(Core::Window::getGLFWCurrentContext(), (int)mouseButton);
	}

	double* getMousePosition()
	{
		double mousePos[2];
		glfwGetCursorPos(Core::Window::getGLFWCurrentContext(), &mousePos[0], &mousePos[1]);
		return mousePos;
	}

	double getMouseX()
	{
		double* mousePos = getMousePosition();
		return mousePos[0];
	}

	double getMouseY()
	{
		double* mousePos = getMousePosition();
		return mousePos[1];
	}
}