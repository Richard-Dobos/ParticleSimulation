#include "Scene.h"

#include "glew.h"
#include "entity/snapshot.hpp"
#include "gtc/matrix_transform.hpp"

namespace Core::Engine
{
	Scene::Scene(std::string sceneName)
		:m_SceneName(sceneName) 
	{
	}

	entt::entity Scene::createEntity()
	{
		entt::entity entity = m_Registry.create();

		m_EntityList.push_back(entity);

		return entity;
	}

	entt::entity Scene::createEntity(std::string groupName)
	{
		entt::entity entity = m_Registry.create();

		m_EntityList.push_back(entity);

		addEntityToGroup(entity, groupName);

		return entity;
	}

	void Scene::deleteEntity(entt::entity entity)
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

	void Scene::addEntityToGroup(entt::entity entity, std::string groupName)
	{
		m_Groups[groupName].emplace_back(entity);
	}

	std::vector<entt::entity> Scene::getEntityGroup(std::string groupName)
	{
		if (!m_Groups.contains(groupName))
		{
			LOG_WARN("Entity Group ({}) couldn't be found.", groupName);
			return {};
		}

		return m_Groups[groupName];
	}

	void Scene::switchMainCamera(Camera2d& camera)
	{
		m_MainCamera = &camera;
	}

	void Scene::updateScene()
	{
		std::chrono::high_resolution_clock::time_point frameStartTime = std::chrono::high_resolution_clock::now();
#ifdef PERFORMANCE_PROFILER

		for (const System& system : m_Systems)
		{
			m_ThreadDispatcher.dispatchTask(system);
		}

		m_DefaultSceneProfiler.profile("updateEntities", [this]() { updateEntities();	});
		m_DefaultSceneProfiler.profile("SceneRenderPass", [this]() { sceneRenderPass(); });

#else
		for (const System& system : m_Systems)
		{
			m_ThreadDispatcher.dispatchTask(system);
		}

		updateEntities();
		sceneRenderPass();
#endif
		std::chrono::high_resolution_clock::time_point frameEndTime = std::chrono::high_resolution_clock::now();

		m_DeltaTime = std::chrono::duration<double, std::milli>(frameEndTime - frameStartTime).count();
	}

	void Scene::sceneRenderPass()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto view = m_Registry.view<Engine::Components::Transform, Engine::Components::Color, Engine::Components::Render>();

		uint32_t quadsToDraw = view.size_hint();

		if (quadsToDraw < 1)
		{
			glFlush();
			return;
		}

		m_Renderer.beginBatch();
		view.each([&](const Components::Transform& transform, const Components::Color& color)
			{
				m_Renderer.DrawQuad(transform, color);
			});
		m_Renderer.endBatch();
			
		glFlush();
	}

	void Scene::updateEntities()
	{
		

		if (!m_DeletedEntities.empty())
		{
			for (entt::entity entity : m_DeletedEntities)
			{
				m_Registry.destroy(entity);
			}

			m_DeletedEntities.clear();
		}

		//copyRegistry(m_Registry, m_RenderingRegistry);
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

	double Scene::getDeltaTime()
	{
		return m_DeltaTime;
	}

	void Scene::changeActiveShader(uint32_t shaderID)
	{
		//if (m_ActiveShaderID != shaderID)
		{
			Utils::Shader* shader = m_Shaders.at(shaderID);

			m_ActiveShaderID = shaderID;

			Utils::Shader::bind(m_ActiveShaderID);

			shader->setUniformMat4x4("u_View", m_MainCamera->getViewMatrix());
			shader->setUniformMat4x4("u_Proj", m_MainCamera->getProjectionMatrix());
		}
	}

	void Scene::addSystem(const System& ECSSystem)
	{
		m_Systems.emplace_back(ECSSystem);
	}

	uint64_t Scene::getEntityCount() const
	{
		return m_EntityList.size();
	}

	std::vector<entt::entity> Scene::getEntityList() const
	{
		return m_EntityList;
	}
}