#include <format>
#include <memory>
#include <vector>
#include <filesystem>
#include <thread>

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

	// 100 x 100 squares
	// 1280 / 100 = 12.8
	// 720 / 100 = 7.2

    void generateSquares(const Window::Window& window, Engine::Scene& currentScene, int numberOfQuadsHor, int numberOfQuadsVer)
    {
        for (uint16_t y = 0; y < numberOfQuadsVer; y++)
        {
            for (uint16_t x = 0; x < numberOfQuadsHor; x++)
            {
                uint8_t r = static_cast<uint8_t>(std::rand() % 256);
                uint8_t g = static_cast<uint8_t>(std::rand() % 256);
                uint8_t b = static_cast<uint8_t>(std::rand() % 256);

                entt::entity entity = currentScene.createEntity();

				float squareWidth = window.getWindowProperties()->Width / numberOfQuadsHor;
				float squareHeight = window.getWindowProperties()->Height / numberOfQuadsVer;

                currentScene.getRegistry().emplace<Engine::Transform>(entity, Engine::Transform{ { x * squareWidth, y * squareHeight, 0}, {squareWidth, squareHeight, 0}, 0.0f });
                currentScene.getRegistry().emplace<Engine::Color>(entity, Engine::Color{ r, g, b, 255 });
                currentScene.getRegistry().emplace<Engine::Render>(entity, Engine::Render{});
            }
        }
    }

    void Application::Update()
    {
        Window::Window coreWindow(RESOLUTION[0], RESOLUTION[1], "Demo APP", false);

		coreWindow.setEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

        Window::setGLFWWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        Window::setGLFWWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

        std::string vertShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultVertexShader.glsl";
        std::string fragShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultFragShader.glsl";

        Utils::Shader defaultShader(vertShaderPath.c_str(), fragShaderPath.c_str());
        
        Engine::Scene m_CurrentScene("Main Scene");
        Engine::Camera2d mainCamera(Engine::Transform({ 0.0f, 0.0f, 0.0f }), RESOLUTION[0], RESOLUTION[1], coreWindow.getWindowProperties()->AspectRatio, -1.0f, 1.0f);

        m_CurrentScene.switchMainCamera(mainCamera);

        m_CurrentScene.addShader(defaultShader);

        bool fizzEffect = false;

        int numberOfQuadsHor = 1280;
        int numberOfQuadsVer = 720;

		generateSquares(coreWindow, m_CurrentScene, numberOfQuadsHor, numberOfQuadsVer);

        LOG_TRACE("Number of Entities: {}", m_CurrentScene.getEntityCount());

        {
            Utils::Timer timer;

            while (!Core::Window::windowShouldClose(coreWindow))
            {
                timer.startTimerPeriod();
				
                glClear(GL_COLOR_BUFFER_BIT);

				if (Input::isKeyPressed(Input::KeyCode::F))
				{
					fizzEffect = true;
				}

				if (Input::isKeyPressed(Input::KeyCode::S))
				{
					fizzEffect = false;
				}

                if (fizzEffect)
                {
                    for (entt::entity entity : m_CurrentScene.getEntityList())
                    {
                        m_CurrentScene.deleteEntity(entity);
                    }

                    generateSquares(coreWindow, m_CurrentScene, numberOfQuadsHor, numberOfQuadsVer);
                }

                m_CurrentScene.updateScene();

                glfwSwapBuffers(coreWindow.getGLFWwindow());
                
                glFlush();
                glfwPollEvents();

                const float duration = timer.endTimerPeriod();
				LOG_PERFORMANCE("Frame Time: {}ms | FPS: {}", duration, 1000 / duration);
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
