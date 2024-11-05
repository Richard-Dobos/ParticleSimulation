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

        Window::setGLFWWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        Window::setGLFWWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

        std::string vertShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultVertexShader.glsl";
        std::string fragShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultFragShader.glsl";
        
        Utils::Shader defaultShader(vertShaderPath.c_str(), fragShaderPath.c_str());
        defaultShader.bind();

        Engine::Scene m_CurrentScene("Main Scene");
        Engine::Camera2d mainCamera(Engine::Transform({ 0.0f, 0.0f, 0.0f }), RESOLUTION[0], RESOLUTION[1], coreWindow.getWindowProperties()->AspectRatio, -1.0f, 1.0f);

        m_CurrentScene.createMainCamera(mainCamera);

        m_CurrentScene.addShader(defaultShader);
        m_CurrentScene.m_ActiveShaderID = defaultShader.getShaderProgramID();
        
        float squareWidth = 1.0f;
        float squareHeight = 1.0f;

		m_CurrentScene.m_Ecs.RegisterComponent<Engine::Transform>();
		m_CurrentScene.m_Ecs.RegisterComponent<Engine::Color>();
		m_CurrentScene.m_Ecs.RegisterComponent<Engine::Render>();

        for (uint16_t x = 0; x < 2; x++)
        {
            for (uint16_t y = 0; y < 2; y++)
            {   
                uint8_t r = static_cast<uint8_t>(std::rand() % 256);
                uint8_t g = static_cast<uint8_t>(std::rand() % 256);
                uint8_t b = static_cast<uint8_t>(std::rand() % 256);

                Core::Engine::EntityID et = m_CurrentScene.createEntity();

                m_CurrentScene.m_Ecs.Add<Engine::Transform>(et, Engine::Transform{ { x * squareWidth, y * squareHeight, 0 }, { squareWidth, squareHeight, 0 }, 0.0f });
				m_CurrentScene.m_Ecs.Add<Engine::Color>(et, Engine::Color{ r, g, b, 255 });
                m_CurrentScene.m_Ecs.Add<Engine::Render>(et, Engine::Render{});
            }
        }

        defaultShader.setUniformMat4x4("u_View", mainCamera.getViewMatrix());
        defaultShader.setUniformMat4x4("u_Proj", mainCamera.getProjectionMatrix());

        double mousePos[2];
        
        {
            Utils::Timer timer;
            
            while (!Core::Window::windowShouldClose(coreWindow))
            {
                timer.startTimerPeriod();
                glClear(GL_COLOR_BUFFER_BIT);

                glfwGetCursorPos(coreWindow.getGLFWwindow(), &mousePos[0], &mousePos[1]);

                m_CurrentScene.updateScene();

                glfwSwapBuffers(coreWindow.getGLFWwindow());

                glFlush();
                glfwPollEvents();
                timer.endTimerPeriod();
            }
        }

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
