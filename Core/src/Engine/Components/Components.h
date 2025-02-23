#pragma once

#include "glm.hpp"
#include "string"

namespace Core::Engine::Components
{
	struct Render
	{

	};

	struct Shader
	{
		std::string source;
	};

	struct Transform
	{
		glm::vec3 pos, scale;
		float rotation;
	};

	struct Color
	{
		uint8_t r, g, b, a;
	};

	struct ShaderProgram
	{
		uint32_t shaderID;
	};

	struct Material
	{
		glm::vec3 Albedo;
		
		float Roughness;
		float Metallic;
		float Speculart;
	};
}
