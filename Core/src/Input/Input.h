#pragma once

#include "Core.h"
#include "KeyCodes.h"

namespace Core::Input
{
	CORE_API bool isKeyPressed(KeyCode key);
	CORE_API bool isMouseButtonPressed(KeyCode mouseButton);

	CORE_API double* getMousePosition();
	CORE_API double getMouseX();
	CORE_API double getMouseY();
}