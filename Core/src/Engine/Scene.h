#pragma once

#include <map>
#include <memory>
#include <vector>

#include "GameObject.h"

namespace Core::Engine
{
	class Scene 
	{
	public:
		Scene();
		~Scene();
		
	private:
		//std::map<uint64_t, Component> m_ComponentsID;
		std::map<uint64_t, GameObject> m_GameObjects;
	};
}