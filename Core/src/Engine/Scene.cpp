#include "Scene.h"

#include "glfw3.h"

namespace Core::Engine
{
	void Scene::createGameObject(const Transform& transform, const Color& color)
	{
		uint64_t UID = Utils::xorshf96();
		
		while (m_GameObjects.contains(UID))
			UID = Utils::xorshf96();

		m_GameObjects[UID];
		m_GameObjects[UID].UID = UID;
		m_GameObjectsTransforms[UID] = transform;
		m_GameObjectsColors[UID] = color;
	}

	void Scene::updateScene()
	{
		sceneRenderPass();
	}
	
	inline void Scene::sceneRenderPass()
	{
		m_Renderer.beginBatch();

		for (const auto& gameObject : m_GameObjects)
		{
			const Transform* tf = &m_GameObjectsTransforms[gameObject.first];
			const Color* cl = &m_GameObjectsColors[gameObject.first];

			m_Renderer.DrawQuad({ tf->pos.x, tf->pos.y, 1.0f }, tf->scale, cl->r, cl->g, cl->b, cl->a);
		}

		m_Renderer.endBatch();
	}
}