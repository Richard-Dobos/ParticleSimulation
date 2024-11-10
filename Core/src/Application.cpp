#include <format>
#include <memory>
#include <vector>
#include <filesystem>

#include "Application.h"
#include "Input/Input.h"
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
		std::string testFragShaderPath = findAssetFolder() + "\\DefaultShaders\\Test.glsl";

        Utils::Shader defaultShader(vertShaderPath.c_str(), fragShaderPath.c_str());
		Utils::Shader testShader(vertShaderPath.c_str(), testFragShaderPath.c_str());
        
        Engine::Scene m_CurrentScene("Main Scene");
        Engine::Camera2d mainCamera(Engine::Transform({ 0.0f, 0.0f, 0.0f }), RESOLUTION[0], RESOLUTION[1], coreWindow.getWindowProperties()->AspectRatio, -1.0f, 1.0f);

        m_CurrentScene.switchMainCamera(mainCamera);

        m_CurrentScene.addShader(defaultShader);
		m_CurrentScene.addShader(testShader);

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

        LOG_TRACE("Number of Entities: {}", m_CurrentScene.getEntityCount());

        {
            Utils::Timer timer;

            while (!Core::Window::windowShouldClose(coreWindow))
            {
                timer.startTimerPeriod();
				
                glClear(GL_COLOR_BUFFER_BIT);

                if (Input::isKeyPressed(Input::KeyCode::T))
                {
                    m_CurrentScene.changeActiveShader(testShader.getShaderProgramID());
                }

				if (Input::isKeyPressed(Input::KeyCode::D))
				{
                    m_CurrentScene.changeActiveShader(defaultShader.getShaderProgramID());
				}

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
		//LOG_INFO("Event: {}", e.toString());
    }

    std::string Application::findAssetFolder() const
    {
        std::filesystem::path currentPath = std::filesystem::current_path();
		LOG_INFO("Current Path: {}", currentPath.string());

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
			LOG_INFO("Asset Path: {}", assetPath.string());
            return assetPath.string();
        }

        return currentPath.string();
    }
}
