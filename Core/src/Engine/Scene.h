#pragma once

#include <map>
#include <memory>
#include <vector>
#include <random>
#include <unordered_map>

#include "GameObject.h"
#include "Renderer/Renderer2d.h"
#include "Utils/RandomNumberGenerator.h"

namespace Core::Engine
{
	class Scene 
	{
	public:
		Scene();
		~Scene();

		void createGameObject(GameObject gameObject);
		void updateScene();

	private:
		Renderer::Renderer2d m_Renderer;

		std::unordered_map<uint64_t, GameObject> m_GameObjects;
		std::unordered_map<uint64_t, Transform> m_GameObjectsTransforms;
		std::unordered_map<uint64_t, Material> m_GameObjectMaterials;
	};
}