#include "Window.h"

namespace Core::Window
{
	Window::Window(uint16_t WindowWidth, uint16_t WindowHeight, const std::string& name, bool vsync)
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
			std::cout << GREEN_BACKGROUND << BLACK << std::format("GLFW Window Initialized\nName: {}\nWidth: {}\nHeight: {}\nScale: {}\nV-sync: {}\n",
				m_WindowProperties.Name, m_WindowProperties.Width, m_WindowProperties.Height, m_WindowProperties.AspectRatio, m_WindowProperties.Vsync) << RESET;
			glfwMakeContextCurrent(m_GLFWWindow);
		}

		if (glewInit() != GLEW_OK)
		{
			std::cerr << RED_BACKGROUND << BLACK << "Failed to initialize GLEW.\n" << RESET;
			glfwTerminate();
		}

		else
		{
			m_WindowProperties.GraphicsDevice = std::string((const char*)glGetString(GL_RENDERER));
			m_WindowProperties.OpenGLVersion = std::string((const char*)glGetString(GL_VERSION));
			m_WindowProperties.GLSLVersion = std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
			
			std::cout << GREEN_BACKGROUND << BLACK << std::format("OpenGL Initialized\nGraphics Device: {}\nOpenGL Version: {}\nGLSL Version: {}\n", m_WindowProperties.GraphicsDevice, m_WindowProperties.OpenGLVersion, m_WindowProperties.GLSLVersion) << RESET;
		}

		glfwSwapInterval(m_WindowProperties.Vsync);
		glViewport(0, 0, m_WindowProperties.Width, m_WindowProperties.Height);

		glfwSetWindowUserPointer(m_GLFWWindow, &m_WindowProperties);

		glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow* window, int width, int height)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::WindowResizeEvent event(width, height);

			windowProperties->Width = width;
			windowProperties->Height = height;
			windowProperties->AspectRatio = (float)width / (float)height;
			windowProperties->windowEventCallbackFn(event);
		});

		glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow* window)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::WindowCloseEvent event;
			windowProperties->windowEventCallbackFn(event);
		});

		glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
				
			switch (action)
			{
				case GLFW_PRESS:
				{
					Event::KeyPressedEvent event(key, 0);
					windowProperties->windowEventCallbackFn(event);
					break;
				}
				case GLFW_RELEASE:
				{
					Event::KeyReleasedEvent event(key);
					windowProperties->windowEventCallbackFn(event);
					break;
				}
				case GLFW_REPEAT:
				{
					Event::KeyPressedEvent event(key, 1);
					windowProperties->windowEventCallbackFn(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_GLFWWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::KeyTypedEvent event(keycode);
			windowProperties->windowEventCallbackFn(event);
		});

		glfwSetMouseButtonCallback(m_GLFWWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					Event::MouseButtonPressedEvent event(button);
					windowProperties->windowEventCallbackFn(event);
					break;
				}
				case GLFW_RELEASE:
				{
					Event::MouseButtonReleasedEvent event(button);
					windowProperties->windowEventCallbackFn(event);
					break;
			}
			}
		});
	
		glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::MouseScrolledEvent event(xOffset, yOffset);
			windowProperties->windowEventCallbackFn(event);
		});

		glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::MouseMovedEvent event(xPos, yPos);
			windowProperties->windowEventCallbackFn(event);
		});
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