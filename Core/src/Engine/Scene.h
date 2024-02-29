#pragma once

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
		std::vector<GameObject> m_GameObjects;
	};
}