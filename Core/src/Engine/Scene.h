#pragma once

#include <mutex>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

#include "Core.h"

#include "entity/registry.hpp"

#include "Camera.h"
#include "Utils/Timer.h"
#include "Utils/Shader.h"
#include "Utils/Profiler.h"
#include "Renderer/Renderer2d.h"
#include "Components/Components.h"
#include "Utils/ThreadDispatcher.h"
#include "Utils/RandomNumberGenerator.h"


namespace Core::Engine
{
	class CORE_API Scene
	{
		using System = std::function<void()>;

	public:
		Scene(std::string sceneName);
		~Scene() = default;

		entt::entity createEntity();
		entt::entity createEntity(std::string groupName);
		
		void deleteEntity(entt::entity entity);
		
		void addEntityToGroup(entt::entity entity, std::string groupName);
		std::vector<entt::entity> getEntityGroup(std::string groupName);

		void switchMainCamera(Camera2d& camera);
		
		void addShader(Utils::Shader& shader);
		void addSystem(const System& ECSSystem);
		void changeActiveShader(uint32_t shaderID);

		void updateScene();
		double getDeltaTime();

		uint64_t getEntityCount() const;
		std::vector<entt::entity> getEntityList() const;

		std::string getSceneName() const { return m_SceneName; }

		entt::registry& getRegistry() { return m_Registry; }
		Utils::ThreadDispatcher& getThreadDispatcher() { return m_ThreadDispatcher; }
		Utils::Profiler& getDefaultSceneProfiler() { return m_DefaultSceneProfiler; }

	private:
		void sceneRenderPass();
		void updateEntities();

	private:
		double m_DeltaTime = 0.0;
		
		entt::registry m_Registry;
		entt::registry m_RenderingRegistry;

		uint32_t m_ActiveShaderID = 0;
		std::string m_SceneName;

		Utils::ThreadDispatcher m_ThreadDispatcher;
		Utils::Profiler m_DefaultSceneProfiler;

		Renderer::Renderer2d m_Renderer;
		Engine::Camera2d* m_MainCamera;

		std::vector<entt::entity> m_EntityList;
		std::vector<entt::entity> m_DeletedEntities;
		std::vector<System> m_Systems;
		std::mutex m_RenderDataMutex;

		std::unordered_map<std::string, std::vector<entt::entity>> m_Groups;

		std::unordered_map<uint32_t, Utils::Shader*> m_Shaders;
	};
}