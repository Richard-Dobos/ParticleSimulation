#include <format>
#include <chrono>
#include <filesystem>

#include "Application.h"
#include "Utils/Shader.h"
#include "Utils/Timer.h"
#include "Renderer/Renderer2d.h"

namespace Core
{
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    void Application::Update()
    { 
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
        glfwSwapInterval(0);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo APP", NULL, NULL);

        glViewport(0, 0, mode->width, mode->height);

        if (!window) 
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        // Make the window's context current
        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK) {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            glfwTerminate();
        }

        std::string vertShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultVertexShader.glsl";
        std::string fragShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultFragShader.glsl";

        Utils::Shader defaultShader(vertShaderPath.c_str(), fragShaderPath.c_str());
        defaultShader.bind();

        Renderer::Renderer2d renderer;

        uint64_t frameCount = 0;

        float time = 0;
		auto start = std::chrono::steady_clock::now();

        defaultShader.setUniform2f("Resolution", { WINDOW_WIDTH, WINDOW_HEIGHT });

        // Main rendering loop
        while (!glfwWindowShouldClose(window)) 
        {
            Utils::Timer timer;
            glClear(GL_COLOR_BUFFER_BIT);

            defaultShader.setUniform1f("Time", glfwGetTime());

			renderer.beginBatch();
            renderer.DrawQuad({ -1.0f, -1.0f, 1.0f }, { 2, 2 }, 255, 255, 255, 255);
            renderer.endBatch();
            
            frameCount++;
            
            glfwSwapBuffers(window);

            glFlush();
            glfwPollEvents();
        }

        auto end = std::chrono::steady_clock::now();
        auto timeTaken = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        float AVG =  frameCount / timeTaken;

        std::cout << GREEN_BACKGROUND << BLACK << std::format("\nAverage FPS: {}\nAverage Frame Time: {}ms", AVG, timeTaken * 1000.0f / frameCount) << RESET;
      
        glfwTerminate();
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
