#pragma once

#include <string>
#include <memory>
#include <format>
#include <iostream>

#include "Core.h"
#include "glew.h"
#include "glfw3.h"

namespace Core::Window
{
	static bool s_glewInitialazed = false;
	static bool s_GLFWInitialized = false;

	struct CORE_API WindowProperties
	{
		uint16_t Width;
		uint16_t Height;

		bool Vsync;
		float Scale;
		std::string Name;

		WindowProperties(std::string const& name = "Default Window Handle", uint32_t windwoWidth = 720, uint32_t windowHeight = 480, bool vsync = true)
			:Width(windwoWidth), Height(windowHeight), Vsync(vsync), Scale((float)windwoWidth / (float)windowHeight), Name(name) {}
	};

	class CORE_API Window
	{
	public:
		Window(uint16_t windowWidth, uint16_t windowHeight, const std::string& name, bool vsync);
		Window(WindowProperties& windowProperties) : Window(windowProperties.Width, windowProperties.Height, windowProperties.Name, windowProperties.Vsync) {}
		~Window();

		GLFWwindow* getGLFWwindow() const { return m_GLFWWindow; }

	private:
		WindowProperties m_WindowProperties;

		GLFWwindow* m_GLFWWindow;
		GLFWmonitor* m_GLFWPrimaryMonitor;
		const GLFWvidmode* m_GLFWPrimaryMonitorVidMode;
	};

	CORE_API int windowShouldClose(const Window& window);
	CORE_API void setGLFWWindowHint(int hint, int value);
	CORE_API void setGLFWCurrentContext(GLFWwindow* window);
	CORE_API GLFWwindow* getGLFWCurrentContext();
}