#pragma once

#include "KeyCodes.h"
#include "Window/Window.h"

namespace Core::Input
{
	CORE_API bool isKeyPressed(KeyCode key);
	CORE_API bool isMouseButtonPressed(KeyCode mouseButton);

	CORE_API double* getMousePosition();
	CORE_API double getMouseX();
	CORE_API double getMouseY();
}