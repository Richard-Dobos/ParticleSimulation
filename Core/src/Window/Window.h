#pragma once

#include <string>
#include <memory>
#include <format>
#include <iostream>
#include <functional>

#include "Core.h"
#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

#include "glew.h"
#include "glfw3.h"

namespace Core::Window
{
	using EventCallbackFn = std::function<void(Event::Event&)>;
	
	static bool s_glewInitialized = false;
	static bool s_GLFWInitialized = false;

	struct CORE_API WindowProperties
	{
		uint16_t Width;
		uint16_t Height;

		bool Vsync;
		float AspectRatio;

		std::string Name;
		std::string GraphicsDevice;
		std::string OpenGLVersion;
		std::string GLSLVersion;

		const char* WindowAppIconFilePath = nullptr;

		EventCallbackFn windowEventCallbackFn;

		WindowProperties(std::string const& name = "Default Window Handle", uint16_t windowWidth = 720, uint16_t windowHeight = 480, bool vsync = true)
			:Width(windowWidth), Height(windowHeight), Vsync(vsync), AspectRatio((float)windowWidth / (float)windowHeight), Name(name) {}
	};

	class CORE_API Window
	{
	public:
		Window(uint16_t windowWidth, uint16_t windowHeight, const std::string& name, bool vsync);
		Window(const WindowProperties& windowProperties);
		~Window();

		GLFWwindow* getGLFWwindow() const { return m_GLFWWindow; }
		const WindowProperties* getWindowProperties() const;

		void setGLFWwindowName(const char* windowTitle);
		void setWindowAppIcon(const char* iconFilePath);
		void setEventCallback(const EventCallbackFn& callback) { m_WindowProperties.windowEventCallbackFn = callback; }

	private:
		void init();
		void setGLFWCallbacks();

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