#pragma once

#include "glm.hpp"

namespace Core::Engine
{
	struct Transform
	{
		glm::vec2 pos;
		glm::vec2 scale;
	};

	struct Material
	{
		glm::vec3 Albedo;
		float Roughness;
		float Metallic;
		float Speculart;
	};
}
