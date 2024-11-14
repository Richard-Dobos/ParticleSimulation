#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

#include "Core.h"

#include "ECS.h"
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
	public:
		Scene(std::string sceneName)
			:m_SceneName(sceneName) {}
		~Scene() = default;

		EntityID createEntity();
		void deleteEntity(EntityID entity);
		
		void switchMainCamera(Camera2d& camera);
		
		void addShader(Utils::Shader& shader);
		void changeActiveShader(uint32_t shaderID);

		void updateScene();

		const size_t getSizeB() const;
		uint64_t getEntityCount() const;
		std::vector<EntityID> getEntityList() const;

		ECS& getECS() { return m_ECS; }

		void addSystem(const std::function<void()>& systemFunction);

	private:
		inline void sceneRenderPass();
		inline void updateEntities();

	private:
		ECS m_ECS;

		uint32_t m_ActiveShaderID = 0;

		std::string m_SceneName;

		Renderer::Renderer2d m_Renderer;
		Engine::Camera2d* m_MainCamera;

		std::vector<EntityID> m_EntityList;
		std::vector<EntityID> m_DeletedEntities;
		std::vector<std::function<void()>> m_Systems;
		std::unordered_map<uint32_t, Core::Utils::Shader*> m_Shaders;
	};
}