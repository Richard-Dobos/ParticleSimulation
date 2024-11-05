#include "Scene.h"

#include "glfw3.h"
#include "gtc/matrix_transform.hpp"

namespace Core::Engine
{
	EntityID Scene::createEntity()
	{
		EntityID entity = m_Ecs.CreateEntity();

		m_EntityList.push_back(entity);

		return entity;
	}

	void Scene::deleteEntity(EntityID entity)
	{
		//m_Ecs.DeleteEntity(entity);
	}
	void Scene::createMainCamera(Camera2d& camera)
	{
		m_MainCamera = &camera;
	}

	void Scene::updateScene()
	{
		sceneRenderPass();
	}
	
	inline void Scene::sceneRenderPass()
	{
		m_Renderer.beginBatch();

		m_Ecs.ForEach<Render>([&](EntityID id, Render& render)
		{
			const Transform* tf = &m_Ecs.Get<Transform>(id);
			const Color* cl = &m_Ecs.Get<Color>(id);
		
			m_Renderer.DrawQuad(*tf, *cl);
		});

		/*for (const auto& gameObject : m_GameObjects)
		{
			const Transform* tf = &m_GameObjectsTransforms[gameObject.first];
			const Color* cl = &m_GameObjectsColors[gameObject.first];

			m_Renderer.DrawQuad(*tf, *cl);
		}*/

		m_Renderer.endBatch();
	}	

	void Scene::addShader(Core::Utils::Shader& shader)
	{
		m_Shaders[shader.getShaderProgramID()] = &shader;
	}

	const uint32_t Scene::getSizeB() const
	{
		uint32_t size = 0;

		return size;
	}
}