#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>


#include "Core.h"

#include "entity/registry.hpp"

#include "Camera.h"
#include "Utils/Shader.h"
#include "Utils/Timer.h"
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
		Scene(std::string sceneName)
			:m_SceneName(sceneName) {}
		~Scene() = default;

		entt::entity createEntity();
		void deleteEntity(entt::entity entity);
		
		void switchMainCamera(Camera2d& camera);
		
		void addShader(Utils::Shader& shader);
		void addSystem(const System& ECSSystem);
		void changeActiveShader(uint32_t shaderID);

		void updateScene();

		uint64_t getEntityCount() const;
		std::vector<entt::entity> getEntityList() const;

		entt::registry& getRegistry() { return m_Registry; }
		Utils::ThreadDispatcher& getThreadDispatcher() { return m_ThreadDispatcher; }

	private:
		inline void sceneRenderPass();
		inline void updateEntities();

	private:
		entt::registry m_Registry;

		uint32_t m_ActiveShaderID = 0;
		std::string m_SceneName;

		Utils::ThreadDispatcher m_ThreadDispatcher;
		Renderer::Renderer2d m_Renderer;
		Engine::Camera2d* m_MainCamera;

		std::vector<entt::entity> m_EntityList;
		std::vector<entt::entity> m_DeletedEntities;
		std::vector<System> m_Systems;
		std::unordered_map<uint32_t, Utils::Shader*> m_Shaders;
	};
}