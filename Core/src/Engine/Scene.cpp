#include "Scene.h"

namespace Core::Engine
{
	void Scene::createGameObject(GameObject gameObject)
	{
		m_GameObjects[Utils::xorshf96()] = gameObject;

	}

	void Scene::updateScene()
	{
	}
}