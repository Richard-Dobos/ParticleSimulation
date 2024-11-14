#include "Scene.h"

#include "glfw3.h"
#include "gtc/matrix_transform.hpp"

namespace Core::Engine
{
	EntityID Scene::createEntity()
	{
		EntityID entity = m_ECS.CreateEntity();

		m_EntityList.push_back(entity);

		return entity;
	}

	void Scene::deleteEntity(EntityID entity)
	{
		if (!m_EntityList.empty())
		{
			for (uint64_t i = 0; i < m_EntityList.size(); i++)
			{
				if (m_EntityList[i] == entity)
				{
					m_DeletedEntities.push_back(entity);
					std::swap(m_EntityList[i], m_EntityList.back());
					m_EntityList.pop_back();

					break;
				}
			}
		}
	}
	void Scene::switchMainCamera(Camera2d& camera)
	{
		m_MainCamera = &camera;
	}

	void Scene::updateScene()
	{
		//m_MainThread = std::thread(&Scene::updateEntities, this);
		//m_RenderThread = std::thread(&Scene::sceneRenderPass, this);
		updateEntities();
		sceneRenderPass();
	}
	
	inline void Scene::sceneRenderPass()
	{
		m_Renderer.beginBatch();
		
		std::vector<uint8_t> renderingData;

		uint32_t quadsToDraw = 0;
	
		m_ECS.ForEach<Transform, Color>([&](Transform transform, Color color)
		{
			m_Renderer.DrawQuad(transform, color);
			quadsToDraw++;
		});

		m_Renderer.endBatch();

		m_Renderer.drawCalls = 0;
	}	

	inline void Scene::updateEntities()
	{
		if (!m_DeletedEntities.empty())
		{
			for (EntityID entity : m_DeletedEntities)
			{
				m_ECS.DeleteEntity(entity);
			}

			m_DeletedEntities.clear();
		}

		for (const std::function<void()>& system : m_Systems)
		{
			system();
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

	void Scene::addSystem(const std::function<void()>& systemFunction)
	{
		m_Systems.emplace_back(systemFunction);
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

	std::vector<EntityID> Scene::getEntityList() const
	{
		return m_EntityList;
	}
}