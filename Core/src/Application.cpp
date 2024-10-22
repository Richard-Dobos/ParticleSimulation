#include <format>
#include <chrono>
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
        Window::Window coreWindow(RESOLUTION[0], RESOLUTION[1], "Demo APP", true);

        Window::setGLFWWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        Window::setGLFWWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

        std::string vertShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultVertexShader.glsl";
        std::string fragShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultFragShader.glsl";
        
        Utils::Shader defaultShader(vertShaderPath.c_str(), fragShaderPath.c_str());
        defaultShader.bind();

        Engine::Scene m_CurrentScene("Main Scene");
        Engine::Camera2d mainCamera(Engine::Transform({ 0.0f, 0.0f, 1.0f }), RESOLUTION[0], RESOLUTION[1], coreWindow.getWindowProperties()->AspectRatio, 0.0f, 10.0f);

        //defaultShader.setUniformVector("u_Resolution", Utils::ShaderUniformDataType::SHADER_UNIFORM_VEC2, &RESOLUTION, 2);
        m_CurrentScene.createGameObject(Engine::Transform({ 0.0f, 0.0f, 0.0f }, { 720.0f, 720.0f, 0.0f }, 0.0f), { 255, 255, 255, 255});
       
        defaultShader.setUniformMat4x4("u_View", mainCamera.getViewMatrix());
        defaultShader.setUniformMat4x4("u_Proj", mainCamera.getProjectionMatrix());

        double mousePos[2];
        
        while(!Core::Window::windowShouldClose(coreWindow)) 
        {
            glClear(GL_COLOR_BUFFER_BIT);
          
            glfwGetCursorPos(coreWindow.getGLFWwindow(), &mousePos[0], &mousePos[1]);

            m_CurrentScene.updateScene();

            glfwSwapBuffers(coreWindow.getGLFWwindow());

            glFlush();
            glfwPollEvents();
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
