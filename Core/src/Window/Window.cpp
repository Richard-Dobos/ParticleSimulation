#include "Window.h"

#include "Events/Event.h"

namespace Core::Window
{
	Window::Window(uint16_t WindowWidth, uint16_t WindowHeight, const std::string& name, bool vsync)
	{
		m_WindowProperties.Height = WindowHeight;
		m_WindowProperties.Width = WindowWidth;
		m_WindowProperties.AspectRatio = (float)WindowWidth / (float)WindowHeight;
		m_WindowProperties.Name = name;
		m_WindowProperties.Vsync = vsync;
		m_WindowProperties.WindowEventCallbackFn = std::bind(Event::onEvent, std::placeholders::_1);
		
		init();
		setGLFWCallbacks();
	}

	Window::Window(const WindowProperties& windowProperties)
		:m_WindowProperties(windowProperties)
	{
		init();
		setGLFWCallbacks();
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::setGLFWCallbacks()
	{
		glfwSetWindowUserPointer(m_GLFWWindow, &m_WindowProperties);

		glfwSetWindowSizeCallback(m_GLFWWindow, [](GLFWwindow* window, int width, int height)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::WindowResizeEvent event(width, height);
			
			windowProperties->Width = width;
			windowProperties->Height = height;
			windowProperties->AspectRatio = (float)width / (float)height;
			windowProperties->WindowEventCallbackFn(event);

			glViewport(0, 0, width, height);
		});

		glfwSetWindowCloseCallback(m_GLFWWindow, [](GLFWwindow* window)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::WindowCloseEvent event;
			windowProperties->WindowEventCallbackFn(event);
		});

		glfwSetWindowFocusCallback(m_GLFWWindow, [](GLFWwindow* window, int focused)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::WindowFocusEvent event(focused);
			windowProperties->WindowEventCallbackFn(event);
		});

		glfwSetKeyCallback(m_GLFWWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				Event::KeyPressedEvent event(key, 0);
				windowProperties->WindowEventCallbackFn(event);
				break;
			}
			case GLFW_RELEASE:
			{
				Event::KeyReleasedEvent event(key);
				windowProperties->WindowEventCallbackFn(event);
				break;
			}
			case GLFW_REPEAT:
			{
				Event::KeyPressedEvent event(key, 1);
				windowProperties->WindowEventCallbackFn(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_GLFWWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::KeyTypedEvent event(keycode);
			windowProperties->WindowEventCallbackFn(event);
		});

		glfwSetMouseButtonCallback(m_GLFWWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
			{
				Event::MouseButtonPressedEvent event(button);
				windowProperties->WindowEventCallbackFn(event);
				break;
			}
			case GLFW_RELEASE:
			{
				Event::MouseButtonReleasedEvent event(button);
				windowProperties->WindowEventCallbackFn(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_GLFWWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::MouseScrolledEvent event(xOffset, yOffset);
			windowProperties->WindowEventCallbackFn(event);
		});

		glfwSetCursorPosCallback(m_GLFWWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowProperties* windowProperties = (WindowProperties*)glfwGetWindowUserPointer(window);
			Event::MouseMovedEvent event(xPos, yPos);
			windowProperties->WindowEventCallbackFn(event);
		});
	}

	void Window::init()
	{
		if (!glfwInit())
		{
			LOG_ERROR("Failed to initialize GLFW!");
		}

		m_GLFWPrimaryMonitor = glfwGetPrimaryMonitor();
		m_GLFWPrimaryMonitorVidMode = glfwGetVideoMode(m_GLFWPrimaryMonitor);
		m_GLFWWindow = glfwCreateWindow(m_WindowProperties.Width, m_WindowProperties.Height, m_WindowProperties.Name.c_str(), nullptr, nullptr);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

		if (!m_GLFWWindow)
			LOG_ERROR("Failed to initialize GLFW Window!");
		if (!m_GLFWPrimaryMonitor)
			LOG_ERROR("Failed to find Primary Monitor!");
		if (!m_GLFWPrimaryMonitorVidMode)
			LOG_ERROR("Failed to get monitor's video modes!");

		if (m_GLFWWindow)
		{
			LOG_INFO("GLFW Window Initialized\n\tName: {}\n\tWidth: {}\n\tHeight: {}\n\tScale: {}\n\tV-sync: {}", m_WindowProperties.Name, m_WindowProperties.Width, m_WindowProperties.Height, m_WindowProperties.AspectRatio, m_WindowProperties.Vsync);
			glfwMakeContextCurrent(m_GLFWWindow);
		}

		if (glewInit() != GLEW_OK)
		{
			LOG_ERROR("Failed to initialize GLEW!");
			glfwTerminate();
		}

		else
		{
			m_WindowProperties.GraphicsDevice = std::string((const char*)glGetString(GL_RENDERER));
			m_WindowProperties.OpenGLVersion = std::string((const char*)glGetString(GL_VERSION));
			m_WindowProperties.GLSLVersion = std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

			LOG_INFO("OpenGL Initialized\n\tGraphics Device: {}\n\tOpenGL Version: {}\n\tGLSL Version: {}", m_WindowProperties.GraphicsDevice, m_WindowProperties.OpenGLVersion, m_WindowProperties.GLSLVersion);
		}

		glfwSwapInterval(m_WindowProperties.Vsync);
		glViewport(0, 0, m_WindowProperties.Width, m_WindowProperties.Height);
	}

	void Window::setEventCallback(const EventCallbackFn& event)
	{
		m_WindowProperties.WindowEventCallbackFn = event;
	}

	uint16_t* Window::getWindowHeightPtr()
	{
		return &m_WindowProperties.Height;
	}

	uint16_t* Window::getWindowWidthPtr()
	{
		return &m_WindowProperties.Width;
	}

	const WindowProperties* Window::getWindowProperties() const
	{
		return &m_WindowProperties;
	}

	void Window::setGLFWwindowName(const char* windowTitle)
	{
		m_WindowProperties.Name = windowTitle;
		glfwSetWindowTitle(m_GLFWWindow, windowTitle);
	}

	void Window::setWindowAppIcon(const char* iconFilePath)
	{
		m_WindowProperties.WindowAppIconFilePath = iconFilePath;
	}
	
	int windowShouldClose(const Window& window)
	{
		return glfwWindowShouldClose(window.getGLFWwindow());
	}

	void swapBuffers(const Window* window)
	{
		glfwSwapBuffers(window->getGLFWwindow());
	}

	void setGLFWWindowHint(int hint, int value)
	{
		glfwWindowHint(hint, value);
	}

	void setGLFWCurrentContext(GLFWwindow* window)
	{
		glfwMakeContextCurrent(window);
	}

	void setGLFWCurrentContext(const Window* newWindowContext)
	{
		glfwMakeContextCurrent(newWindowContext->getGLFWwindow());
	}

	GLFWwindow* getGLFWCurrentContext()
	{
		return glfwGetCurrentContext();
	}
}