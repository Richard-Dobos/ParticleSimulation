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
		for (uint64_t i = 0; i < m_EntityList.size(); i++)
		{
			if (m_EntityList[i] == entity)
			{
				m_DeletedEntities.push_back(entity);
				std::swap(m_EntityList[i], m_EntityList.back());
				
				break;
			}
		}
	}
	void Scene::switchMainCamera(Camera2d& camera)
	{
		m_MainCamera = &camera;
	}

	void Scene::updateScene()
	{
		updateEntities();
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

	inline void Scene::updateEntities()
	{
		for (EntityID entity : m_DeletedEntities)
		{
			 m_Ecs.DeleteEntity(entity);
		}

		if (!m_DeletedEntities.empty())
		{
			m_DeletedEntities.clear();
		}
	}

	void Scene::addShader(Utils::Shader& shader)
	{
		if (m_ActiveShaderID == 0)
		{
			m_ActiveShaderID = shader.getShaderProgramID();

			Utils::Shader::bind(m_ActiveShaderID);

			shader.setUniformMat4x4("u_View", m_MainCamera->getViewMatrix());
			shader.setUniformMat4x4("u_Proj", m_MainCamera->getProjectionMatrix());
		}

		m_Shaders[shader.getShaderProgramID()] = &shader;
	}

	void Scene::changeActiveShader(uint32_t shaderID)
	{
		if (m_ActiveShaderID != shaderID)
		{
			Utils::Shader* shader = m_Shaders.at(shaderID);

			m_ActiveShaderID = shaderID;

			Utils::Shader::bind(m_ActiveShaderID);

			shader->setUniformMat4x4("u_View", m_MainCamera->getViewMatrix());
			shader->setUniformMat4x4("u_Proj", m_MainCamera->getProjectionMatrix());
		}
	}

	const size_t Scene::getSizeB() const
	{
		size_t sceneSize = 0;

		return sceneSize;
	}

	uint64_t Scene::getEntityCount() const
	{
		return m_EntityList.size();
	}
}