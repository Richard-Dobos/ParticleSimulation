#include <iostream>
#include <filesystem>

#include "CoreEngine.h"
#include "CoreEngineUtils.h"

using namespace Core;

std::string findAssetFolder()
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

void generateGrid(float cellSize, uint32_t gridSizeX, uint32_t gridSizeY, Engine::Scene& scene)
{
    for (uint32_t i = 0; i < gridSizeY; i++)
    {
        if (i % 2 == 0)
        {
            for (uint32_t j = 0; j < gridSizeX; j++)
            {
                if (j % 2 == 0)
                {
                    entt::entity gridCell = scene.createEntity("Grid");
                    scene.getRegistry().emplace<Engine::Components::Transform>(gridCell, Engine::Components::Transform{{ cellSize * j, cellSize * i, -0.05f }, { cellSize, cellSize, 0 }, 0.1f});
                    scene.getRegistry().emplace<Engine::Components::Color>(gridCell, Engine::Components::Color{ static_cast<uint8_t>(cellSize * j), static_cast<uint8_t>(cellSize * i), 120, 255 });
                    scene.getRegistry().emplace<Engine::Components::Render>(gridCell, Engine::Components::Render{});
                }
            }
        }
    }
}

void genGrid(float cellSize, uint32_t gridSizeX, uint32_t gridSizeY, Engine::Scene& scene)
{
    for (uint32_t i = 0; i < gridSizeY; i++)
    {
        for (uint32_t j = 0; j < gridSizeX; j++)
        {
            entt::entity gridCell = scene.createEntity("Grid");
            scene.getRegistry().emplace<Engine::Components::Transform>(gridCell, Engine::Components::Transform{ { cellSize * j, cellSize * i, -0.05f }, { cellSize, cellSize, 0 }, 0.1f });
            scene.getRegistry().emplace<Engine::Components::Color>(gridCell, Engine::Components::Color{ static_cast<uint8_t>(cellSize * j), static_cast<uint8_t>(cellSize * i), 120, 255 });
            scene.getRegistry().emplace<Engine::Components::Render>(gridCell, Engine::Components::Render{});
    }
    }
}

Engine::Components::Color generateColor()
{
    double time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count() / 1500.0;

    uint8_t red = static_cast<uint8_t>((std::sin(time) + 1) * 127.5);
    uint8_t green = static_cast<uint8_t>((std::sin(time + 2.0) + 1) * 127.5);   
    uint8_t blue = static_cast<uint8_t>((std::sin(time + 4.0) + 1) * 127.5);

    return Engine::Components::Color{ red, green, blue, 255 };
}

int main()
{
    Window::Window mainWindow(1920, 1080, "Demo APP", false);
    
    Window::setGLFWWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::string vertShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultVertexShader.glsl";
    std::string fragShaderPath = findAssetFolder() + "\\DefaultShaders\\DefaultFragShader.glsl";
    std::string depthBufferFragShader = findAssetFolder() + "\\DefaultShaders\\FragShader.glsl";

    Utils::Shader defaultShader(vertShaderPath.c_str(), fragShaderPath.c_str());
    Utils::Shader depthBufferShader(vertShaderPath.c_str(), depthBufferFragShader.c_str());

    Engine::Scene CurrentScene("Main Scene");
    Engine::Scene MovementTestScene("Movement SandBox");

    Engine::Camera2d mainCamera(Engine::Components::Transform({ 0.0f, 0.0f, 0.0f }), mainWindow.getWindowWidthPtr(), mainWindow.getWindowHeightPtr(), mainWindow.getWindowProperties()->AspectRatio, -0.1f, 1.0f);

    CurrentScene.switchMainCamera(mainCamera);
    CurrentScene.addShader(defaultShader);
    CurrentScene.addShader(depthBufferShader);
    
	MovementTestScene.switchMainCamera(mainCamera);
	MovementTestScene.addShader(defaultShader);
	MovementTestScene.addShader(depthBufferShader);

    entt::entity player = MovementTestScene.createEntity();
	MovementTestScene.getRegistry().emplace<Engine::Components::Color>(player, Engine::Components::Color{ static_cast<uint8_t>(255), static_cast<uint8_t>(100), 120, 255 });
	MovementTestScene.getRegistry().emplace<Engine::Components::Transform>(player, Engine::Components::Transform{ { 0.0f, 0.0f, 0.0f }, { 100, 100, 0 }, 0.0 });
	MovementTestScene.getRegistry().emplace<Engine::Components::Render>(player, Engine::Components::Render{});

    generateGrid(10, mainWindow.getWindowProperties()->Width / 10, mainWindow.getWindowProperties()->Height / 10, CurrentScene);
    
    Engine::Scene* activeScene = &CurrentScene;
    float movementSpeed = 20;
    
    std::vector<entt::entity> players;
    uint32_t playerIndex = 0;

    players.emplace_back(player);

    {
        Utils::Timer timer;
        
        while (!Core::Window::windowShouldClose(mainWindow))
        {
            timer.startTimerPeriod();   
            
            activeScene->updateScene();

            if (players.size() > 0)
				activeScene->getRegistry().get<Engine::Components::Color>(players[playerIndex]) = generateColor();

            if (Input::isMouseButtonPressed(Input::KeyCode::MOUSE_BUTTON_1))
            {
				entt::entity rect = activeScene->createEntity();
				
                double* mousePos = Input::getMousePosition();

                activeScene->getRegistry().emplace<Engine::Components::Transform>(rect, Engine::Components::Transform{ { mousePos[0]-50, mousePos[1]-50, 0.0f}, {100, 100, 0}, 0.0});
				activeScene->getRegistry().emplace<Engine::Components::Color>(rect, Engine::Components::Color{ static_cast<uint8_t>(255), static_cast<uint8_t>(100), 120, 255 });
				activeScene->getRegistry().emplace<Engine::Components::Render>(rect, Engine::Components::Render{});

                players.emplace_back(rect);
            }

            if (Input::isKeyPressed(Input::KeyCode::N) && playerIndex != 0)
				playerIndex--;

            if (Input::isKeyPressed(Input::KeyCode::M) && playerIndex != players.size() - 1)
				playerIndex++;

            else
            {
                if (Input::isKeyPressed(Input::KeyCode::LEFT_SHIFT))
                    movementSpeed = 100;
                else
					movementSpeed = 20;


                if (Input::isKeyPressed(Input::KeyCode::W))
                    activeScene->getRegistry().get<Engine::Components::Transform>(players[playerIndex]).pos.y -= movementSpeed * activeScene->getDeltaTime();

                if (Input::isKeyPressed(Input::KeyCode::S))
                    activeScene->getRegistry().get<Engine::Components::Transform>(players[playerIndex]).pos.y += movementSpeed * activeScene->getDeltaTime();


                if (Input::isKeyPressed(Input::KeyCode::D))
                    activeScene->getRegistry().get<Engine::Components::Transform>(players[playerIndex]).pos.x += movementSpeed * activeScene->getDeltaTime();

                if (Input::isKeyPressed(Input::KeyCode::A))
                    activeScene->getRegistry().get<Engine::Components::Transform>(players[playerIndex]).pos.x -= movementSpeed * activeScene->getDeltaTime();
            }

			if (Input::isKeyPressed(Input::KeyCode::TWO))
                activeScene = &MovementTestScene;

            if (Input::isKeyPressed(Input::KeyCode::T))
                activeScene->changeActiveShader(depthBufferShader.getShaderProgramID());

            if (Input::isKeyPressed(Input::KeyCode::R))
                activeScene->changeActiveShader(defaultShader.getShaderProgramID());

            std::string newWindowTitle = std::format("{} Entities: {} | Players: {} | Current Player Index: {}", activeScene->getSceneName(), activeScene->getEntityCount(), players.size(), playerIndex);

            mainWindow.setGLFWwindowName(newWindowTitle.c_str());
            Event::pollEvents();
            Window::swapBuffers(&mainWindow);

            const float duration = timer.endTimerPeriod();
            LOG_PERFORMANCE("Frame Time: {}ms | FPS: {}", duration, 1000 / duration);
        }
    }
}