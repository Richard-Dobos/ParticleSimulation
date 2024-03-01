#pragma once

#include "glm.hpp"

namespace Core::Engine
{
	class Component
	{
	public:
		Component() = delete;
		~Component() = delete;
	};

	struct Transform
ga	{
		glm::vec2 pos;
		glm::vec2 scale;
	};
}
