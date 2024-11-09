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
		m_Ecs.DeleteEntity(entity);
	}
	void Scene::switchMainCamera(Camera2d& camera)
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
		
		std::vector<uint8_t> renderingData;

		uint32_t quadsToDraw = 0;
	
		m_Ecs.ForEach<Transform, Color>([&](Transform transform, Color color)
		{
			m_Renderer.DrawQuad(transform, color);
			quadsToDraw++;
		});

		m_Renderer.endBatch();

		m_Renderer.drawCalls = 0;
	}	

	void Scene::addShader(Utils::Shader& shader)
	{
		m_Shaders[shader.getShaderProgramID()] = &shader;
	}

	const uint32_t Scene::getSizeB() const
	{
		uint32_t size = 0;

		return size;
	}

	uint32_t Scene::getEntityCount() const
	{
		return m_EntityList.size();
	}
}