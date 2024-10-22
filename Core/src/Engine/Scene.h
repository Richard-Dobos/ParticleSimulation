#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "Core.h"

#include "GameObject.h"
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

		void createGameObject(const GameObject& gameObject);
		void createGameObject(const Transform& transfrom, const Color& color);
		void updateScene();

	private:
		inline void sceneRenderPass();

	private:
		const char* m_SceneName;

		Renderer::Renderer2d m_Renderer;

		std::unordered_map<uint64_t, GameObject> m_GameObjects;
		std::unordered_map<uint64_t, Transform> m_GameObjectsTransforms;
		std::unordered_map<uint64_t, Color> m_GameObjectsColors;
	};
}