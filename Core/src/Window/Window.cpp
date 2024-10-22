#include "Window.h"

namespace Core::Window
{
	Window::Window(uint16_t WindowWidth, uint16_t WindowHeight, std::string const& name, bool vsync)
	{
		m_WindowProperties.Height = WindowHeight;
		m_WindowProperties.Width = WindowWidth;
		m_WindowProperties.AspectRatio = (float)WindowWidth / (float)WindowHeight;
		m_WindowProperties.Name = name;
		m_WindowProperties.Vsync = vsync;

		if (!glfwInit())
			std::cerr << RED_BACKGROUND << BLACK << "Failed to initialize GLFW!\n" << RESET;

		m_GLFWPrimaryMonitor = glfwGetPrimaryMonitor();
		m_GLFWPrimaryMonitorVidMode = glfwGetVideoMode(m_GLFWPrimaryMonitor);
		m_GLFWWindow = glfwCreateWindow(m_WindowProperties.Width, m_WindowProperties.Height, m_WindowProperties.Name.c_str(), nullptr, nullptr);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

		if (!m_GLFWWindow)
			std::cerr << RED_BACKGROUND << BLACK << "Failed to initialize GLFW Window!\n" << RESET;
		if (!m_GLFWPrimaryMonitor)
			std::cerr << RED_BACKGROUND << BLACK << "Failed to find Primary Monitor!\n" << RESET;
		if (!m_GLFWPrimaryMonitorVidMode)
			std::cerr << RED_BACKGROUND << BLACK << "Failed to get monitor's video modes!\n" << RESET;

		if (m_GLFWWindow)
		{
			std::cout << GREEN_BACKGROUND << BLACK << std::format("GLFW Window Initialized\nName: {}\nWidth: {}\nHeight: {}\nScale: {}\nVsync: {}\n",
				m_WindowProperties.Name, m_WindowProperties.Width, m_WindowProperties.Height, m_WindowProperties.AspectRatio, m_WindowProperties.Vsync) << RESET;
			glfwMakeContextCurrent(m_GLFWWindow);
		}

		if (glewInit() != GLEW_OK)
		{
			std::cerr << RED_BACKGROUND << BLACK << "Failed to initialize GLEW.\n" << RESET;
			glfwTerminate();
		}

		else
			std::cout << GREEN_BACKGROUND << BLACK << "GLEW Initialized.\n" << RESET;

		glfwSwapInterval(m_WindowProperties.Vsync);
		glViewport(0, 0, m_WindowProperties.Width, m_WindowProperties.Height);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	const WindowProperties* Window::getWindowProperties() const
	{
		return &m_WindowProperties;
	}

	int windowShouldClose(const Window& window)
	{
		return glfwWindowShouldClose(window.getGLFWwindow());
	}

	void setGLFWWindowHint(int hint, int value)
	{
		glfwWindowHint(hint, value);
	}

	void setGLFWCurrentContext(GLFWwindow* window)
	{
		glfwMakeContextCurrent(window);
	}

	GLFWwindow* getGLFWCurrentContext()
	{
		return glfwGetCurrentContext();
	}
}