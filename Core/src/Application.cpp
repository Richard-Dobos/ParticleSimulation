#include <format>
#include <memory>
#include <vector>
#include <filesystem>

#include "Application.h"
#include "Utils/Timer.h"
#include "Utils/Shader.h"
#include "Engine/Scene.h"
#include "Window/Window.h"
#include "Engine/Camera.h"
#include "Engine/Components/Components.h"

namespace Core
{
    const uint16_t RESOLUTION[2] = { 1280, 720 };

    void Application::Update()
    {
        Window::Window coreWindow(RESOLUTION[0], RESOLUTION[1], "Demo APP", false);

		coreWindow.setEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

        Window::setGLFWWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        Window::setGLFWWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

        std::string vertShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultVertexShader.glsl";
        std::string fragShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultFragShader.glsl";
        
        Utils::Shader defaultShader(vertShaderPath.c_str(), fragShaderPath.c_str());
        defaultShader.bind();

        Engine::Scene m_CurrentScene("Main Scene");
        Engine::Camera2d mainCamera(Engine::Transform({ 0.0f, 0.0f, 0.0f }), RESOLUTION[0], RESOLUTION[1], coreWindow.getWindowProperties()->AspectRatio, -1.0f, 1.0f);

        m_CurrentScene.switchMainCamera(mainCamera);

        m_CurrentScene.addShader(defaultShader);
        m_CurrentScene.m_ActiveShaderID = defaultShader.getShaderProgramID();
        
        float squareWidth = 10;
        float squareHeight = 10;

        float divider = 10;

		m_CurrentScene.m_Ecs.RegisterComponent<Engine::Transform>();
		m_CurrentScene.m_Ecs.RegisterComponent<Engine::Color>();
		m_CurrentScene.m_Ecs.RegisterComponent<Engine::Render>();

        for (uint16_t x = 0; x < coreWindow.getWindowProperties()->Width / divider; x++)
        {
            for (uint16_t y = 0; y < coreWindow.getWindowProperties()->Height / divider; y++)
            {   
                uint8_t r = static_cast<uint8_t>(std::rand() % 256);
                uint8_t g = static_cast<uint8_t>(std::rand() % 256);
                uint8_t b = static_cast<uint8_t>(std::rand() % 256);

                Core::Engine::EntityID et = m_CurrentScene.createEntity();

				m_CurrentScene.m_Ecs.Add<Engine::Color>(et, Engine::Color{ r, g, b, 255 });
                m_CurrentScene.m_Ecs.Add<Engine::Transform>(et, Engine::Transform{ { x * squareWidth, y * squareHeight, 0 }, { squareWidth, squareHeight, 0 }, 0.0f });
                m_CurrentScene.m_Ecs.Add<Engine::Render>(et, Engine::Render{});
            }
        }

        std::cout << YELLOW_BACKGROUND << BLACK << std::format("Number of Quads: {}\n", m_CurrentScene.getEntityCount()) << RESET;

        defaultShader.setUniformMat4x4("u_View", mainCamera.getViewMatrix());
        defaultShader.setUniformMat4x4("u_Proj", mainCamera.getProjectionMatrix());

        {
            Utils::Timer timer;

            while (!Core::Window::windowShouldClose(coreWindow))
            {
                timer.startTimerPeriod();
				
                glClear(GL_COLOR_BUFFER_BIT);

                m_CurrentScene.updateScene();
                
                glfwSwapBuffers(coreWindow.getGLFWwindow());
                
                glFlush();
                glfwPollEvents();
                timer.endTimerPeriod();
            }
        }
    }

    void Application::OnEvent(Event::Event& e)
    {
        Event::EventDispatcher dispatcher(e);

		std::cout << YELLOW_BACKGROUND << BLACK << e.toString() << '\n' << RESET;
    }

    std::string Application::findAssetFolder() const
    {
        std::filesystem::path currentPath = std::filesystem::current_path();
        std::cout << WHITE_BACKGROUND << BLACK << "Current Path: " << currentPath.string() << '\n' << RESET;

        std::filesystem::path assetPath;
        bool foundFolder = false;

        for (const auto& component : currentPath) 
        {
            assetPath /= component;

            if (component == "ParticleSimulation") 
            {
                foundFolder = true;
                break;
            }
        }

        assetPath += "\\Assets";

        if (foundFolder) 
        {
            std::cout << GREEN_BACKGROUND << BLACK << "Asset path: " << assetPath << '\n';
            return assetPath.string();
        }

        std::cout << RED_BACKGROUND << BLACK << "Couldn't find folder \"Assets\"\n";
        return currentPath.string();
    }
}
