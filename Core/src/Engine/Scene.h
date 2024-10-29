#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "Core.h"

#include "ECS.h"
#include "Camera.h"
#include "GameObject.h"
#include "Utils/Shader.h"
#include "Renderer/Renderer2d.h"
#include "Components/Components.h"
#include "Utils/RandomNumberGenerator.h"

namespace Core::Engine
{
	class CORE_API Scene
	{
	public:
		Scene(const char* sceneName)
			:m_SceneName(sceneName) {}
		~Scene() = default;

		//void createEntity(const Transform& transform, const Color& color);
		EntityID createEntity();
		void deleteEntity(EntityID entity);
		void createMainCamera(Camera2d& camera);

		void updateScene();

		void addShader(Core::Utils::Shader& shader);

		const uint32_t getSizeB() const;

	public:
		uint32_t m_ActiveShaderID;
		ECS m_Ecs;

	private:
		inline void sceneRenderPass();
		
	private:
		const char* m_SceneName;

		Renderer::Renderer2d m_Renderer;
		Engine::Camera2d* m_MainCamera;

		std::vector<EntityID> m_EntityList;
		std::unordered_map<uint32_t, Core::Utils::Shader*> m_Shaders;
		
		/*
		std::unordered_map<uint64_t, GameObject> m_GameObjects;
		std::unordered_map<uint64_t, Transform> m_GameObjectsTransforms;
		std::unordered_map<uint64_t, Color> m_GameObjectsColors;
		*/
	};
}