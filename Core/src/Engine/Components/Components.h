#pragma once

#include "glm.hpp"

namespace Core::Engine
{
	struct Transform
	{
		glm::vec3 pos, scale;
		float rotation;
	};
	
	struct Color
	{
		uint8_t r, g, b, a;
	};

	struct Material
	{
		glm::vec3 Albedo;
		float Roughness;
		float Metallic;
		float Speculart;
	};
}
